// ---------------------------------- //
// reproducibility.cc
// Check simulation reproducibility
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
    program.add_argument("-i", "--input").default_value(std::string("mc.root"))
        .help("input mc filename including mc TTree with .root");
    program.add_argument("-j", "--input2").default_value(std::string("mc2.root"))
        .help("input mc filename 2 including mc TTree with .root");
    program.add_argument("-t", "--tree").default_value(std::string("tree"))
        .help("TTree name");
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
    auto inFileName2 = program.get<std::string>("--input2");
    TString treename = program.get<std::string>("--tree");

    spdlog::info("The basic_check has been started, a stopwatch is on, now.");
    spdlog::stopwatch stopwatch;
    gitinfo();

    // Load input tree (processed)
    auto inFile = TFile::Open(TString(inFileName));
    if (inFile == nullptr) {spdlog::error("No such file, {}.", inFileName); std::exit(1);}
    auto inFile2 = TFile::Open(TString(inFileName2));
    if (inFile2 == nullptr) {spdlog::error("No such file, {}.", inFileName2); std::exit(1);}

    auto tree1 = dynamic_cast<TTree*>(inFile->Get(treename));
    if (tree1 == nullptr) {spdlog::error("No {} in file", treename); std::exit(1);}
    auto tree2 = dynamic_cast<TTree*>(inFile2->Get(treename));
    if (tree2 == nullptr) {spdlog::error("No {} in file", treename); std::exit(1);}

    ULong64_t nEntries = tree1->GetEntries();
    if (nEntries != tree2->GetEntries()){
        spdlog::error("Entries are not match."); 
        std::exit(1);
    }

    spdlog::info("Created files are match", stopwatch);

    return 0;
}
