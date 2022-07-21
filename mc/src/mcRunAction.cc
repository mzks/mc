#include "mcRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include <spdlog/spdlog.h>

mcRunAction::mcRunAction()
{}


mcRunAction::~mcRunAction()
{}


void mcRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
    spdlog::info("Run {:d} start.", aRun->GetRunID());

    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);

    NbOfEventToBeProcessed = aRun->GetNumberOfEventToBeProcessed();
    spdlog::info("{:d} events to be processed.", NbOfEventToBeProcessed);

}


void mcRunAction::EndOfRunAction(const G4Run* aRun)
{
    G4int NbOfEvents = aRun->GetNumberOfEvent();
    if (NbOfEvents == 0) return;
    spdlog::info("Run {:d} end.", aRun->GetRunID());

    
}
