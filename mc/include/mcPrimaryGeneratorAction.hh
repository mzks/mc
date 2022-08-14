
#ifndef mcPrimaryGeneratorAction_h
#define mcPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4GeneralParticleSource.hh"
#include "mcPrimaryGeneratorMessenger.hh"

class G4ParticleGun;
class G4ParticleTable;
class G4GeneralParticleSource;
class G4Event;
class mcDetectorConstruction;

class mcPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    mcPrimaryGeneratorAction();
    ~mcPrimaryGeneratorAction();

public:
    void GeneratePrimaries(G4Event*);
    void SelectAction(G4int i);
    void SetDetector(G4VUserDetectorConstruction* detector){
        this->detector = detector;
    };
    
private:
    G4ParticleGun*              particleGun;	  //pointer a to G4 class
    G4ParticleTable*			particleTable;
    G4GeneralParticleSource*     gps;
    mcPrimaryGeneratorMessenger* pMessenger;
    G4VUserDetectorConstruction* detector = nullptr;
    G4int selectedAction = 0;
    // 0 -> mcParticleGun
    // 1 -> GPS
    // 2 -> user

    void GenerateCustom(G4Event* anEvent);

};


#endif


