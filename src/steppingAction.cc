#include "G4Track.hh"
#include "G4Step.hh"
#include "G4UnitsTable.hh"

#include "steppingAction.hh"
#include "eventAction.hh"

steppingAction::steppingAction(eventAction *currentEvent)
  : evtAction(currentEvent)
{;}

steppingAction::~steppingAction()
{;}


// Anything included in this function is performed each time the
// particle takes a step
void steppingAction::UserSteppingAction(const G4Step *currentStep)
{
  // These commands, in conjunction with eventAction, allows scintTest
  // to collect the energy deposited FOR EACH STEP in each piece of
  // scintillator

  // Get the current particle track
  G4Track *currentTrack = currentStep -> GetTrack();
   
  // Determine what volume the particle is currently in
  G4String currentVolumeName = currentTrack -> GetVolume() -> GetName();
  
  // If in scintillator tile 1, get the energy deposited...
  G4double EDepTile1 = 0.;
  if(currentVolumeName == "scint_p1")
    EDepTile1 = currentStep -> GetTotalEnergyDeposit();
  
  // If in scintillator tile 2, get the energy deposited...
  //G4double EDepTile2 = 0.;
  //if(currentVolumeName == "scint_p2")
  //  EDepTile2 = currentStep -> GetTotalEnergyDeposit();
    
  
  // Once the energy deposited per step has been collected, send it to 
  // eventAction, which stores the TOTAL energy per each event.  
  evtAction -> AddEnergyDep(EDepTile1);
}

