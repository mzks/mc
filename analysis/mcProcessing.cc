#include <TROOT.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include <TString.h>

#include <vector>
#include <string>
#include <filesystem>

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <argparse/argparse.hpp>

#include <common.hh>

int main(int argc, char** argv){

    std::cout << GIT_SHA1 << std::endl;

    argparse::ArgumentParser program("mc");
    program.add_argument("-i", "--input").default_value(std::string("mc.root")).help("input mc filename with .root");
    program.add_argument("-o", "--output").default_value(std::string("processed.root")).help("output file name with .root");
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        spdlog::error("Your arguments is not match. See --help.");
        spdlog::error(err.what());
        std::cout << program.help().str() << std::endl;;
        std::exit(1);
    }

    auto inFileName = program.get<std::string>("--input");
    auto outFileName = program.get<std::string>("--output");
    spdlog::info("Input file is {}.", inFileName);
    spdlog::info("Output file will be generate as {}.", outFileName);

    spdlog::info("The processing has been started, a stopwatch is on, now.");
    spdlog::stopwatch stopwatch;

    // Load input tree (tree)
    auto inFile = TFile::Open(TString(inFileName));
    auto inTree = dynamic_cast<TTree*>(inFile->Get("tree"));

    Int_t           nHit;
    std::vector<double>  *x = 0;
    std::vector<double>  *y = 0;
    std::vector<double>  *z = 0;
    std::vector<double>  *time = 0;
    std::vector<double>  *eIn = 0;
    std::vector<double>  *eDep = 0;
    std::vector<int>     *TrackID = 0;
    std::vector<int>     *copyNo = 0;
    std::vector<int>     *particle = 0;

    TBranch        *b_nHit;
    TBranch        *b_x;
    TBranch        *b_y;
    TBranch        *b_z;
    TBranch        *b_time;
    TBranch        *b_eIn;
    TBranch        *b_eDep;
    TBranch        *b_TrackID;
    TBranch        *b_copyNo;
    TBranch        *b_particle;

    inTree->SetBranchAddress("nHit", &nHit, &b_nHit);
    inTree->SetBranchAddress("x", &x, &b_x);
    inTree->SetBranchAddress("y", &y, &b_y);
    inTree->SetBranchAddress("z", &z, &b_z);
    inTree->SetBranchAddress("time", &time, &b_time);
    inTree->SetBranchAddress("eIn", &eIn, &b_eIn);
    inTree->SetBranchAddress("eDep", &eDep, &b_eDep);
    inTree->SetBranchAddress("TrackID", &TrackID, &b_TrackID);
    inTree->SetBranchAddress("copyNo", &copyNo, &b_copyNo);
    inTree->SetBranchAddress("particle", &particle, &b_particle);

    // make new tree (processed)
    auto outFile = TFile::Open(TString(outFileName), "RECREATE");
    auto outTree = new TTree("processed", "processed");

    Double_t TotalEnergyDeposit;
    Double_t Debug;
    outTree->Branch("TotalEnergyDeposit", &TotalEnergyDeposit, "TotalEnergyDeposit/D" );
    outTree->Branch("Debug", &Debug, "Debug/D");
    outTree->Branch("particle", &particle);

    ULong64_t nEntries = inTree->GetEntries();


    // Event loop
    for(ULong64_t iEntry=0; iEntry<nEntries; ++iEntry){
        inTree->GetEntry(iEntry);

        // Display log at each 10% processing
        if (iEntry == 0) spdlog::info("Initial event 0 has been done.");
        if ((100 * iEntry / nEntries % 10 == 0) && ((100 * iEntry / nEntries) != (100 * (iEntry-1) / nEntries))) {
            spdlog::info("{:d} percent events ({:d}) have been done.", 100 * (iEntry) / nEntries, iEntry);
        }

        // Initialize variables
        TotalEnergyDeposit = 0.;
        Debug = 4.;

        // Make new variables
        for(size_t i=0; i<eDep->size(); ++i){
            TotalEnergyDeposit += eDep->at(i);
        }

        outTree->Fill();
    }
    outTree->Write();
    outFile->Close();

    spdlog::info("The processing has been finished, it took {:.3} seconds.", stopwatch);
    spdlog::info("Size of output root file is {:.1} MB.", std::filesystem::file_size(outFileName) * 1.e-6);

    return 0;
}
