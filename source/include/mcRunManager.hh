
#ifndef mcRunManager_h
#define mcRunMa nager_h 1

#include "G4RunManager.hh"
#include "TTree.h"


class mcRunManager : public G4RunManager {
public:
    mcRunManager();
    TTree* tree;
};

#endif
