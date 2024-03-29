//
//  mcAnalyzer.cc
//  mc
//
//  Created by Mizukoshi Keita on 2018/07/28.
//

#include <vector>

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Types.hh"
#include "Randomize.hh"

#include "TTree.h"
#include "TFile.h"
#include "TParameter.h"

#include "mcAnalyzer.hh"

#include <spdlog/spdlog.h>

#include "common.hh"

mcAnalyzer::mcAnalyzer()
{
}

mcAnalyzer::~mcAnalyzer(){
}
mcAnalyzer::mcAnalyzer(const mcAnalyzer &obj) {
    tree = obj.tree;
}

void mcAnalyzer::SetInit(G4bool isRootIn, TString filenameIn){
    isRoot = isRootIn;
    filename = filenameIn;
}
void mcAnalyzer::Init(){
    
    if(isRoot == true){
        fout = new TFile(filename,"recreate");
        auto git_sha1 = new TNamed("git_sha1", gitinfo("GIT_SHA1"));
        auto git_date = new TNamed("git_date", gitinfo("GIT_DATE"));
        auto git_subject = new TNamed("git_subject", gitinfo("GIT_COMMIT_SUBJECT"));
        auto G4Version = new TNamed("G4Version", G4VersionString);
        auto ROOTVersion = new TNamed("ROOTVersion", gROOT->GetVersion());
        auto physList = new TNamed("physicslist_name", physListName);
        git_sha1->Write();
        git_date->Write();
        git_subject->Write();
        G4Version->Write();
        ROOTVersion->Write();
        physList->Write();

        auto seed = new TParameter<Long_t>("seed", G4Random::getTheSeed());
        seed->Write();
        auto gaps_custom_physics = new TParameter<bool>("gaps_custom_physics", GAPSCustomPhysics);
        gaps_custom_physics->Write();

        tree = new TTree("mc","mc output");
        tree->Branch("nHit",&nHit,"nHit/I");
        tree->Branch("x",&x);
        tree->Branch("y",&y);
        tree->Branch("z",&z);
        tree->Branch("time",&time);
        tree->Branch("eIn",&eIn);
        tree->Branch("eDep",&eDep);
        tree->Branch("TrackID",&trackID);
        tree->Branch("copyNo",&copyNo);
        tree->Branch("particle",&particleID);
    } else {
        // ascii file
        outFile.open(filename, std::ios::out);
        outFile.setf( std::ios:: scientific, std::ios::floatfield );
        outFile << std::setprecision(8);
    }
}

void mcAnalyzer::Fill(int buf1,                     //nHit
                      std::vector<G4double> buf2,   //x
                      std::vector<G4double> buf3,   //y
                      std::vector<G4double> buf4,   //z
                      std::vector<G4double> buf5,   //time
                      std::vector<G4double> buf6,   //eIn
                      std::vector<G4double> buf7,   //eDep
                      std::vector<G4int> buf8,   //TrackID
                      std::vector<G4int> buf9,   //copyNo
                      std::vector<G4int> buf10   //particleID
)
{
    nHit    = buf1;
    x       = buf2;
    y       = buf3;
    z       = buf4;
    time    = buf5;
    eIn     = buf6;
    eDep    = buf7;
    trackID = buf8;
    copyNo  = buf9;
    particleID = buf10;
    
    if (isRoot == true){
        tree->Fill();
        
        x.clear();
        y.clear();
        z.clear();
        time.clear();
        eIn.clear();
        eDep.clear();
        trackID.clear();
        copyNo.clear();
        particleID.clear();
    } else {
        outFile << nHit << std::endl;
        for(int i=0;i<nHit;++i){
            outFile << x.at(i)
            << " "  << y.at(i)
            << " "  << z.at(i)
            << " "  << time.at(i)
            << " "  << eIn.at(i)
            << " "  << eDep.at(i)
            << " "  << trackID.at(i)
            << " "  << copyNo.at(i)
            << " "  << particleID.at(i)
            << std::endl;
        }
    }
}

void mcAnalyzer::Terminate(){
    if (isRoot == true){
        tree->Write();
        fout->Close();
    } else {
        outFile.close();
    }
    
}

void mcAnalyzer::SetFileName(TString name){
    filename = name;
}
