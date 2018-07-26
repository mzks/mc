
#ifndef mcDetectorMessenger_h
#define mcDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class mcDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class mcDetectorMessenger: public G4UImessenger
{
  public:
    mcDetectorMessenger(mcDetectorConstruction* );
   ~mcDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
  G4String GetCurrentValue(G4UIcommand * command);

  private:
    mcDetectorConstruction* mcDetector;
    
    G4UIdirectory*             mcDir;
    G4UIdirectory*             detDir;
    G4UIcmdWithAString*        MaterialCmd;
    G4UIcmdWithADoubleAndUnit* ThicknessCmd;
    G4UIcmdWithADoubleAndUnit* SizeYZCmd;
    G4UIcmdWithADoubleAndUnit* ArmCmd;
    G4UIcmdWithADoubleAndUnit* MaxStepCmd;
    G4UIcmdWithADoubleAndUnit* MagFieldCmd;
    G4UIcmdWithADoubleAndUnit* PESizeCmd;
    G4UIcmdWithADoubleAndUnit* BoronSizeCmd;
};


#endif

