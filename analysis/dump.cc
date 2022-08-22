// ---------------------------------- //
// dump.cc
// dump tree
// Usage : yes q | dump -i mc.root -t tree
// This code is silent.
// should not show outputs 
// except of Scan()
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

#include <argparse/argparse.hpp>

int main(int argc, char** argv) {
    argparse::ArgumentParser program("dump");
    program.add_argument("-i", "--input").default_value(std::string("mc.root"))
        .help("input mc filename including mc TTree with .root");
    program.add_argument("-t", "--tree").default_value(std::string("mc"))
        .help("TTree name");
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::exit(1);
    }

    auto inFileName = program.get<std::string>("--input");
    TString treename = program.get<std::string>("--tree");

    // Load input tree (processed)
    auto inFile = TFile::Open(TString(inFileName));
    auto tree1 = dynamic_cast<TTree*>(inFile->Get(treename));

    tree1->Scan();
    return 0;
}
