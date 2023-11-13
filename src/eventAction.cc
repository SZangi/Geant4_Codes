#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include "G4RunManagerKernel.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "eventAction.hh"
#include "eventActionMessenger.hh"

eventAction::eventAction()
{
  // Create a messenger to allow user commands 
  eventMessenger = new eventActionMessenger(this);
  
  // This sets the name of the default MuSE output data file
  eventOutput.open("defaultOutput.dat",std::ofstream::trunc);
  
  // This is a boolean 'on' or 'off' switch to control data ouput
  dataOutputSwitch = false;
}


eventAction::~eventAction()
{ 
  eventOutput.close();
  delete eventMessenger;
}


// Anything included in this function is performed before each event
// is tracked through the geometry
void eventAction::BeginOfEventAction(const G4Event *)
{
  // Initialization per event.  We need to reset to the total energy
  // collected at the beginning of each event
  TotalEnergyDepTile1 = 0.;
}


// Anything included in this function is performed at the very end of
// each event's lifetime.
void eventAction::EndOfEventAction(const G4Event *)
{
  // If the user has turned data output 'on', then do this!
  if(dataOutputSwitch and (TotalEnergyDepTile1 > 0))
    eventOutput << TotalEnergyDepTile1/keV << std::endl;
}
