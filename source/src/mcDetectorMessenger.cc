#include "mcDetectorMessenger.hh"
#include "mcDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


mcDetectorMessenger::mcDetectorMessenger(
                                           mcDetectorConstruction* mcDet)
:mcDetector(mcDet)
{ 
  mcDir = new G4UIdirectory("/mc/");
  mcDir->SetGuidance("UI commands of this example");
  detDir = new G4UIdirectory("/mc/det/");
  detDir->SetGuidance("UI commands for detector setup");
  
  MaterialCmd = new G4UIcmdWithAString("/mc/det/setMaterial",this);
  MaterialCmd->SetGuidance("Select Material of the sensor");
  MaterialCmd->SetParameterName("choice",false);
  MaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  ThicknessCmd = new G4UIcmdWithADoubleAndUnit("/mc/det/setThickness",this);
  ThicknessCmd->SetGuidance("Set Thickness of the sensor");
  ThicknessCmd->SetParameterName("Size",false);
  ThicknessCmd->SetRange("Size>=0.");
  ThicknessCmd->SetUnitCategory("Length");
  ThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SizeYZCmd = new G4UIcmdWithADoubleAndUnit("/mc/det/setSize",this);
  SizeYZCmd->SetGuidance("Set tranverse size of the sensor");
  SizeYZCmd->SetParameterName("Size",false);
  SizeYZCmd->SetRange("Size>0.");
  SizeYZCmd->SetUnitCategory("Length");    
  SizeYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  ArmCmd = new G4UIcmdWithADoubleAndUnit("/mc/det/setArm",this);
  ArmCmd->SetGuidance("Set Lever Arm of the sensor");
  ArmCmd->SetParameterName("Arm",false);
  ArmCmd->SetRange("Arm>0.");
  ArmCmd->SetUnitCategory("Length");    
  ArmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/mc/det/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  MaxStepCmd = new G4UIcmdWithADoubleAndUnit("/mc/det/setMaxStep",this);
  MaxStepCmd->SetGuidance("Set MaxStep ");
  MaxStepCmd->SetParameterName("MaxStep",false);
  MaxStepCmd->SetRange("MaxStep>0.");
  MaxStepCmd->SetUnitCategory("Length");    
  MaxStepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  PESizeCmd = new G4UIcmdWithADoubleAndUnit("/mc/det/setPESize",this);
  PESizeCmd->SetGuidance("Set PE size");
  PESizeCmd->SetParameterName("PESize",false);
  PESizeCmd->SetRange("PESize>0.");
  PESizeCmd->SetUnitCategory("Length");    
  PESizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  BoronSizeCmd = new G4UIcmdWithADoubleAndUnit("/mc/det/setBoronSize",this);
  BoronSizeCmd->SetGuidance("Set Boron size");
  BoronSizeCmd->SetParameterName("BoronSize",false);
  BoronSizeCmd->SetRange("BoronSize>0.");
  BoronSizeCmd->SetUnitCategory("Length");    
  BoronSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}


mcDetectorMessenger::~mcDetectorMessenger()
{
  delete MaterialCmd;
  delete ThicknessCmd;
  delete SizeYZCmd;  
  delete ArmCmd;  
  delete MagFieldCmd;
  delete MaxStepCmd;  
  delete mcDir;  
  delete BoronSizeCmd;
  delete PESizeCmd;
}


void mcDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == MaterialCmd ){
    mcDetector->SetSensorMaterial(newValue);

  } else if( command == ThicknessCmd ){
    mcDetector->SetSensorThickness(
		   ThicknessCmd->GetNewDoubleValue(newValue)
		  );
   
  } else if( command == SizeYZCmd ){
    mcDetector->SetSensorSize(SizeYZCmd->GetNewDoubleValue(newValue));
    

  } else if( command == ArmCmd ){
    mcDetector->SetSensorLeverArm(ArmCmd->GetNewDoubleValue(newValue));
    

  } else if( command == MaxStepCmd ){
    mcDetector->SetMaxStep(MaxStepCmd->GetNewDoubleValue(newValue));
    

  } else if( command == MagFieldCmd ){
    mcDetector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));

  } else if( command == PESizeCmd ){
    mcDetector->SetPESize(PESizeCmd->GetNewDoubleValue(newValue));

  } else if( command == BoronSizeCmd ){
    mcDetector->SetBoronSize(BoronSizeCmd->GetNewDoubleValue(newValue));

  }
}


G4String mcDetectorMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String cv;
  
  if( command==MaterialCmd ){
    cv =  mcDetector->GetSensorMaterial()->GetName();

  } else if( command==ThicknessCmd ){
    cv =  ThicknessCmd->ConvertToString( mcDetector->GetSensorThickness(),"mm");

  } else if( command==SizeYZCmd ){
    cv =  SizeYZCmd->ConvertToString( mcDetector->GetSensorSize(),"mm");

  } else if( command==ArmCmd ){
    cv =  ArmCmd->ConvertToString( mcDetector->GetSensorLeverArm(),"cm");

  } else if( command==MaxStepCmd ){
    cv =  MaxStepCmd->ConvertToString( mcDetector->GetMaxStep(),"mm");

  } else if( command==MagFieldCmd ){
    cv =  MagFieldCmd->ConvertToString( mcDetector->GetFieldValue(),"tesla");

  } else if( command==PESizeCmd ){
    cv =  PESizeCmd->ConvertToString( mcDetector->GetPESize(),"mm");

  } else if( command==BoronSizeCmd ){
    cv =  BoronSizeCmd->ConvertToString( mcDetector->GetBoronSize(),"mm");

  }
  
  return cv;
}

