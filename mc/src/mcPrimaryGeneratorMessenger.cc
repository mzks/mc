#include "mcPrimaryGeneratorMessenger.hh"
#include "mcPrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"


mcPrimaryGeneratorMessenger::mcPrimaryGeneratorMessenger(mcPrimaryGeneratorAction* Gun) :Action(Gun) {

  fDir = new G4UIdirectory("/mcgun/");
  fDir->SetGuidance("Command to set primary generator action");

  fSelectActionCmd = new G4UIcmdWithAString("/mcgun/selectGunAction", this);
  fSelectActionCmd->SetGuidance("Select primary generator action");
  fSelectActionCmd->SetGuidance("pg : (mc) particle gun");
  fSelectActionCmd->SetGuidance("gps : general particle source");
  fSelectActionCmd->SetGuidance("custom : custom user command");
  fSelectActionCmd->SetCandidates("pg gps custom");
  fSelectActionCmd->SetParameterName("guntype", false);
}


mcPrimaryGeneratorMessenger::~mcPrimaryGeneratorMessenger()
{
  delete fSelectActionCmd;
  delete fDir;
}


void mcPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fSelectActionCmd){
    if (newValue == "pg") Action->SelectAction(0);
    if (newValue == "gps") Action->SelectAction(1);
    if (newValue == "custom") Action->SelectAction(2);
  }
}

