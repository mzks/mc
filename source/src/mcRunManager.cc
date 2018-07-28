//
//  mcRunManager.cc
//  mc
//
//  Created by Mizukoshi Keita on 2018/07/27.
//
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "mcRunManager.hh"

#include "TH1D.h"
#include "TTree.h"


mcRunManager::mcRunManager() : G4RunManager()
{
    tree = new TTree("tree","tree");
}
