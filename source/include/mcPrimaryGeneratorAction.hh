
#ifndef mcPrimaryGeneratorAction_h
#define mcPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4ParticleTable;
class G4Event;
class mcDetectorConstruction;

class mcPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    mcPrimaryGeneratorAction(const mcDetectorConstruction*);    
    ~mcPrimaryGeneratorAction();
    
public:
    void GeneratePrimaries(G4Event*);
    
private:
    G4ParticleGun*              particleGun;	  //pointer a to G4  class
    G4ParticleTable*			particleTable;
    //const mcDetectorConstruction* mcDetector;  
};


#endif


