// ------------------------------------ //
// mc.cc
// Geant4 example for experiment
// Author: Mizukoshi keita
// 2018 July 25
// ------------------------------------ //

#include "mcDetectorConstruction.hh"
#include "mcPhysicsList.hh"
#include "mcPrimaryGeneratorAction.hh"
#include "mcRunAction.hh"
#include "mcEventAction.hh"
#include "mcParticleGun.hh"
#include "mcParticleGunMessenger.hh"
#include "mcAnalyzer.hh"

#include "mcRunManager.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#include "QGSP_BERT.hh"
#include <iostream>

int main(int argc,char** argv)
{
    // Detect interactive mode (if no arguments) and define UI session
    //
    G4UIExecutive* ui = 0;
    if ( argc == 1 ) {
        ui = new G4UIExecutive(argc, argv);
    }
    
    // Choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    
    // Construct the default run manager
    mcRunManager * runManager = new mcRunManager;
    
    // Construct the analyzer
    mcAnalyzer* analyzer = new mcAnalyzer();
    analyzer->SetInit(true, "out.root");
    analyzer->Init();
    
    // Set mandatory initialization classes
    mcDetectorConstruction* detector = new mcDetectorConstruction();
    detector->SetAnalyzer(analyzer);
    runManager->SetUserInitialization(detector);
    
    // Physics list
    G4VModularPhysicsList* physicsList = new QGSP_BERT;
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);
    
    // Set user action classes
    G4VUserPrimaryGeneratorAction* gen_action = new mcPrimaryGeneratorAction(detector);
    runManager->SetUserAction(gen_action);
    
    mcRunAction* run_action = new mcRunAction;
    runManager->SetUserAction(run_action);
    
    mcEventAction* event_action = new mcEventAction(run_action);
    runManager->SetUserAction(event_action);
    
    //mcSteppingAction* stepping_action = new mcSteppingAction();
    //runManager->SetUserAction(stepping_action);
    
    //Initialize G4 kernel
    //runManager->Initialize();
    
    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
    
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Process macro or start UI session
    //
    if ( ! ui ) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    
    analyzer->Terminate();
    
    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    
    delete visManager;
    delete analyzer;
    delete runManager;
}

