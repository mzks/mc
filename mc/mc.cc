// ------------------------------------ //
// mc.cc
// Geant4 example for experiment
// Author: Mizukoshi keita
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
#include <string>
#include <filesystem>

#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <argparse/argparse.hpp>

#include <common.hh>


int main(int argc, char** argv) {

    gitinfo();
    spdlog::info("The mc has been started, a stopwatch is on, now.");
    spdlog::stopwatch stopwatch;

    argparse::ArgumentParser program("mc");
    program.add_argument("-m", "--macro")
        .default_value(std::string("run.mac"))
        .help("set macro filename for batch mode");
    program.add_argument("-o", "--output")
        .default_value(std::string("mc.root"))
        .help("output file name with .root");
    program.add_argument("-i", "--interactive")
        .default_value(false).implicit_value(true)
        .help("run as interactive mode, not batch");
    program.add_argument("-s", "--seed")
        .default_value(0).scan<'i', int>()
        .help("run as interactive mode, not batch");
    program.add_argument("-p", "--path-to-macro")
        .default_value(std::string(".:bench"))
        .help("set macro search path with colon-separated list");
    program.add_argument("-a", "--ascii")
        .default_value(false).implicit_value(true)
        .help("Output with ascii file, not root");
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        spdlog::error("Your arguments is not match. See --help.");
        spdlog::error(err.what());
        G4cout << program.help().str() << G4endl;
        std::exit(1);
    }

    auto outFileName = program.get<std::string>("--output");
    spdlog::info("Output file will be generate as {}.", outFileName);

    G4UIExecutive* ui = nullptr;
    if (program["--interactive"] == true) {
        ui = new G4UIExecutive(argc, argv);
        spdlog::info("Mc works on interactive mode.");
    } else {
        spdlog::info("Mc works on batch mode.");
    }
    
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    auto seed = program.get<int>("seed");
    G4Random::setTheSeed(seed);
    spdlog::info("Set seed {}.", seed);

    auto * runManager = new mcRunManager;

    // Construct the analyzer
    auto* analyzer = new mcAnalyzer();
    analyzer->SetInit(program["--ascii"] == false, outFileName);
    analyzer->SetG4VersionString(runManager->GetVersionString());
    analyzer->Init();
    
    // Set mandatory initialization classes
    auto* detector = new mcDetectorConstruction();
    detector->SetAnalyzer(analyzer);
    runManager->SetUserInitialization(detector);
    
    // Physics list
    G4VModularPhysicsList* physicsList = new QGSP_BERT;
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);
    
    // Set user action classes
    G4VUserPrimaryGeneratorAction* gen_action = new mcPrimaryGeneratorAction(detector);
    runManager->SetUserAction(gen_action);
    
    auto* run_action = new mcRunAction;
    runManager->SetUserAction(run_action);
    
    auto* event_action = new mcEventAction(run_action);
    runManager->SetUserAction(event_action);
    
    // auto* stepping_action = new mcSteppingAction();
    // runManager->SetUserAction(stepping_action);
    
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
    
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Process macro or start UI session
    UImanager->ApplyCommand("/control/macroPath " + program.get<std::string>("--path-to-macro"));
    if (!ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = program.get<std::string>("--macro");
        UImanager->ApplyCommand(command+fileName);
    } else {
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

    spdlog::info("The Mc has been finished, it took {:.3} seconds.", stopwatch);
    spdlog::info("Output file was generate as {}.", outFileName);
    spdlog::info("Size of output root file is {:.0} MB.",
                 std::filesystem::file_size(program.get<std::string>("--output")) * 1e-6);

    return 0;
}
