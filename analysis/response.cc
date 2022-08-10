// ---------------------------------- //
// response.cc
// processes MC generated data
// Require: "tree" TTree
// Output : root file including
//          "response" TTree
// ---------------------------------- //
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

#include <common.hh>
#include <smart_loop_logger.hh>

#include <vector>
#include <string>
#include <filesystem>
#include <numeric>

#include <TROOT.h>
#include <ROOT/RDataFrame.hxx>
#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <TParameter.h>
#include <TText.h>
#include <TRandom.h>

#include <argparse/argparse.hpp>

int main(int argc, char** argv) {
    argparse::ArgumentParser program("mc");
    program.add_argument("-i", "--input").default_value(std::string("mc.root")).help("input mc filename with .root");
    program.add_argument("-o", "--output").default_value(std::string("response.root")).help("output file name with .root");
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

    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df(*tree);

    auto sum = [&] (ROOT::RVecD &vec) { return std::accumulate(vec.begin(), vec.end(), 0.); };
    auto calc_r = [&] (ROOT::RVecD &x, ROOT::RVecD &y, ROOT::RVecD &z) {
      std::vector<Double_t> r;
      for(size_t i=0; i<x.size(); ++i){
        r.push_back(std::sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i] ));
      }
      return  r;
    };
    auto calc_trigger_time = [&] (ROOT::RVecD &time){
      std::vector<Double_t> trigger_time;
      for(size_t i=0; i<time.size(); ++i){
        trigger_time.push_back(time.at(i)-time.at(0));
      }
      return trigger_time;
    };

    Double_t smearing_sigma = 0.2;
    TRandom rand;
    auto calc_smearing = [&] (Double_t e){return e + rand.Gaus(0, smearing_sigma);};

    auto df_new =
        df.Define("total_energy", sum, {"eDep"})
          .Define("r", calc_r, {"x", "y", "z"})
          .Define("trigger_time", calc_trigger_time, {"time"})
          .Define("real_energy", calc_smearing, {"total_energy"})
        ;
    df_new.Snapshot("response", "response.root",
                    {"total_energy", "r", "trigger_time", "real_energy"});


    auto outFile = TFile::Open(TString(outFileName), "UPDATE");
    auto input_filename = new TNamed("input_filename", inFileName);
    auto git_sha1 = new TNamed("git_sha1", gitinfo("GIT_SHA1"));
    auto git_date = new TNamed("git_date", gitinfo("GIT_DATE"));
    auto git_subject = new TNamed("git_subject", gitinfo("GIT_COMMIT_SUBJECT"));
    input_filename->Write();
    git_sha1->Write();
    git_date->Write();
    git_subject->Write();
    outFile->Close();

    spdlog::info("The processing has been finished, it took {:.3} seconds.", stopwatch);
    spdlog::info("Size of output root file is {:.1} MB.", std::filesystem::file_size(outFileName) * 1.e-6);

    return 0;
}
