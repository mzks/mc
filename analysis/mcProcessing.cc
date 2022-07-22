// ---------------------------------- //
// mcProcessing.cc
// processes MC generated data
// Require: "tree" TTree
// Output : root file including 
//          "processed" TTree
// ---------------------------------- //

#include <vector>
#include <string>
#include <filesystem>

#include <TROOT.h>
#include <TDirectory.h>
#include <TObject.h>
#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <TParameter.h>
#include <TText.h>

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <argparse/argparse.hpp>

#include <common.hh>
#include <smart_loop_logger.hh>

int main(int argc, char** argv){

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

    spdlog::info("The processing has been started, a stopwatch is on, now.");
    spdlog::stopwatch stopwatch;

    spdlog::info("Git SHA1 is {}.", GIT_SHA1);
    if(GIT_SHA1.find("dirty") != 0){
        spdlog::warn("The sources have not been committed. Changes in your working directory will not be saved.");
    }
    spdlog::info("Git date is {}.", GIT_DATE);
    spdlog::info("Git commit subject is {}.", GIT_COMMIT_SUBJECT);
    spdlog::info("Input file is {}.", inFileName);
    spdlog::info("Output file will be generate as {}.", outFileName);

    // Load input tree (tree)
    auto inFile = TFile::Open(TString(inFileName));
    if (inFile == nullptr) {spdlog::error("No such file, {}.", inFileName); std::exit(1);}
    auto tree = dynamic_cast<TTree*>(inFile->Get("tree"));
    if (tree == nullptr) {spdlog::error("No 'tree' in file"); std::exit(1);}

    #include "branches_tree.hh"

    // make new tree (processed)
    auto outFile = TFile::Open(TString(outFileName), "RECREATE");
    auto processed = new TTree("processed", "processed");

    auto git_sha1 = new TNamed("git_sha1", GIT_SHA1);
    auto git_date = new TNamed("git_date", GIT_DATE);
    auto git_subject = new TNamed("git_subject", GIT_COMMIT_SUBJECT);
    git_sha1->Write();
    git_date->Write();
    git_subject->Write();

    Double_t TotalEnergyDeposit;
    processed->Branch("TotalEnergyDeposit", &TotalEnergyDeposit, "TotalEnergyDeposit/D" );
    processed->Branch("particle", &particle);


    ULong64_t nEntries = tree->GetEntries();

    // Event loop
    for(ULong64_t iEntry=0; iEntry<nEntries; ++iEntry){
        tree->GetEntry(iEntry);
        smart_loop_logger(nEntries, iEntry);

        // Initialize variables
        TotalEnergyDeposit = 0.;

        // Make new variables
        for(size_t i=0; i<eDep->size(); ++i){
            TotalEnergyDeposit += eDep->at(i);
        }

        processed->Fill();
    }

    // Make histograms
    processed->Draw("TotalEnergyDeposit>>hist_TotalEnergyDeposit");
    auto hist_TotalEnergyDeposit = dynamic_cast<TH1*>(gROOT->FindObject("hist_TotalEnergyDeposit"));
    hist_TotalEnergyDeposit->Write();

    processed->Write();
    outFile->Close();

    spdlog::info("The processing has been finished, it took {:.3} seconds.", stopwatch);
    spdlog::info("Size of output root file is {:.1} MB.", std::filesystem::file_size(outFileName) * 1.e-6);

    return 0;
}
