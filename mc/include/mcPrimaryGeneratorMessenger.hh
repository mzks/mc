#ifndef mcPrimaryGeneratorMessenger_h
#define mcPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class mcPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;

class mcPrimaryGeneratorMessenger: public G4UImessenger {
public:
  mcPrimaryGeneratorMessenger(mcPrimaryGeneratorAction*);
  ~mcPrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  mcPrimaryGeneratorAction* Action;
  G4UIdirectory*        fDir;
  G4UIcmdWithAString* fSelectActionCmd;
};


#endif