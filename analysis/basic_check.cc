// ---------------------------------- //
// basic_check.cc
// Check basic produced data
// Require: "processed" TTree
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
    program.add_argument("-i", "--input").default_value(std::string("processed.root"))
        .help("input mc filename including processed TTree with .root");
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

    spdlog::info("The basic_check has been started, a stopwatch is on, now.");
    spdlog::stopwatch stopwatch;
    gitinfo();

    // Load input tree (processed)
    auto inFile = TFile::Open(TString(inFileName));
    if (inFile == nullptr) {spdlog::error("No such file, {}.", inFileName); std::exit(1);}
    auto processed = dynamic_cast<TTree*>(inFile->Get("processed"));
    if (processed == nullptr) {spdlog::error("No 'processed' in file"); std::exit(1);}

    ULong64_t nEntries = processed->GetEntries();


    processed->Draw("TotalEnergyDeposit>>hist_TotalEnergyDeposit");
    auto hist_TotalEnergyDeposit = dynamic_cast<TH1*>(inFile->Get("hist_TotalEnergyDeposit"));

    assert(hist_TotalEnergyDeposit->GetEntries() > 0);

    spdlog::info("The basic_check has been finished, it took {}.", BestUnit(stopwatch.elapsed().count(), "second"));
    return 0;
}
