// ---------------------------------- //
// mcProcessing.cc
// processes MC generated data
// Require: "tree" TTree
// Output : root file including
//          "processed" TTree
// ---------------------------------- //
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

#include <common.hh>
#include <smart_loop_logger.hh>

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

#include <argparse/argparse.hpp>

int main(int argc, char** argv) {
    argparse::ArgumentParser program("mc");
    program.add_argument("-i", "--input").default_value(std::string("mc.root")).help("input mc filename with .root");
    program.add_argument("-o", "--output").default_value(std::string("processed.root")).help("output file name with .root");
    program.add_argument("-c", "--clone").default_value(false).implicit_value(true).help("clone input tree to output file");
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
    gitinfo();

    // Load input tree (tree)
    auto inFile = TFile::Open(TString(inFileName));
    if (inFile == nullptr) {spdlog::error("No such file, {}.", inFileName); std::exit(1);}
    auto tree = dynamic_cast<TTree*>(inFile->Get("tree"));
    if (tree == nullptr) {spdlog::error("No 'tree' in file"); std::exit(1);}

    #include "branches_tree.hh"

    // make new tree (processed)
    auto outFile = TFile::Open(TString(outFileName), "RECREATE");
    auto processed = new TTree("processed", "processed");

    auto input_filename = new TNamed("input_filename", inFileName);
    auto git_sha1 = new TNamed("git_sha1", gitinfo("GIT_SHA1"));
    auto git_date = new TNamed("git_date", gitinfo("GIT_DATE"));
    auto git_subject = new TNamed("git_subject", gitinfo("GIT_COMMIT_SUBJECT"));
    input_filename->Write();
    git_sha1->Write();
    git_date->Write();
    git_subject->Write();

    Double_t TotalEnergyDeposit;
    processed->Branch("TotalEnergyDeposit", &TotalEnergyDeposit, "TotalEnergyDeposit/D");
    processed->Branch("particle", &particle);


    ULong64_t nEntries = tree->GetEntries();

    // Event loop
    for (ULong64_t iEntry=0; iEntry < nEntries; ++iEntry) {
        tree->GetEntry(iEntry);
        smart_loop_logger(nEntries, iEntry);

        // Initialize variables
        TotalEnergyDeposit = 0.;

        // Make new variables
        for (size_t i=0; i < eDep->size(); ++i) {
            TotalEnergyDeposit += eDep->at(i);
        }

        processed->Fill();
    }
    processed->Write();

    // Make histograms
    processed->Draw("TotalEnergyDeposit>>hist_TotalEnergyDeposit");
    auto hist_TotalEnergyDeposit = dynamic_cast<TH1*>(gROOT->FindObject("hist_TotalEnergyDeposit"));
    hist_TotalEnergyDeposit->Write();

    // Clone input tree
    if (program["--clone"] == true) {
        spdlog::info("Clone tree to output file");
        auto new_tree = tree->CloneTree();
        new_tree->Write();
    }

    outFile->Close();

    spdlog::info("The processing has been finished, it took {}.", BestUnit(stopwatch.elapsed().count(), "second"));
    spdlog::info("Size of output root file is {}.", BestUnit(std::filesystem::file_size(outFileName), "B"));

    return 0;
}
