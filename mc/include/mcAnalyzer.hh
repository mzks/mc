//
//  mcAnalyzer.hh
//  mc
//
//  Created by Mizukoshi Keita on 2018/07/28.
//

#ifndef mcAnalyzer_hh
#define mcAnalyzer_hh

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Types.hh"

#include <vector>
#include <fstream>
#include <iomanip>

#include "TTree.h"
#include "TFile.h"
#include "TString.h"


class mcAnalyzer
{
public:
    mcAnalyzer();
    ~mcAnalyzer();
    mcAnalyzer(const mcAnalyzer&);
    
    void SetInit(G4bool, TString);
    
    
    TTree* getTree(){return tree;};
    void Init();
    void Fill(int buf1,                     //nHit
              std::vector<G4double> buf2,   //x
              std::vector<G4double> buf3,   //y
              std::vector<G4double> buf4,   //z
              std::vector<G4double> buf5,   //time
              std::vector<G4double> buf6,   //eIn
              std::vector<G4double> buf7,   //eDep
              std::vector<G4int> buf8,   //TrackID
              std::vector<G4int> buf9,   //copyNo
              std::vector<G4int> buf10   //particleID
    );
    void Terminate();
    void SetFileName(TString);
private:
    
    TTree* tree;
    TString filename;
    G4bool isRoot;
    std::ofstream outFile;

    G4int nHit;
    std::vector<G4double> x;
    std::vector<G4double> y;
    std::vector<G4double> z;
    std::vector<G4double> time;
    std::vector<G4double> eIn;
    std::vector<G4double> eDep;
    std::vector<G4int> trackID;
    std::vector<G4int> copyNo;
    std::vector<G4int> particleID;
    
};


#endif /* mcAnalyzer_hh */
