#ifndef mcParticleGunMessenger_h
#define mcParticleGunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "mcParticleGun.hh"

class mcParticleGun;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;


class mcParticleGunMessenger: public G4UImessenger
{
public:
    mcParticleGunMessenger();
    explicit mcParticleGunMessenger(mcParticleGun*);
    ~mcParticleGunMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    G4String GetCurrentValue(G4UIcommand* command);
private:
    mcParticleGun*		mcPG;
    G4UIdirectory*			cmdDir;
    G4UIcmdWithAnInteger*		vtxCmd;
    G4UIcmdWithAnInteger*		parCmd;
    G4UIcmdWithADoubleAndUnit*  eneCmd;
    
};

#endif

