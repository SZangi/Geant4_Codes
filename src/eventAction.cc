#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include "G4RunManagerKernel.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "eventAction.hh"
#include "runAction.hh"
#include "eventActionMessenger.hh"
#include "G4RunManager.hh"

eventAction::eventAction(runAction* currentrun)
: fRunAction(currentrun)
{
  // Create a messenger to allow user commands 
  eventMessenger = new eventActionMessenger(this);
  
  // This sets the name of the default MuSE output data file
  eventOutput.open("defaultOutput.csv",std::ofstream::trunc);

  // Add a second output file for the processes, so we don't
  // clog up the data one
  //processOutput.open("defaultProcessOutput.csv",std::ofstream::trunc);
  
  // This is a boolean 'on' or 'off' switch to control data ouput
  dataOutputSwitch = false;
}


eventAction::~eventAction()
{ 
  eventOutput.close();
  //processOutput.close();
  delete eventMessenger;
}


// Anything included in this function is performed before each event
// is tracked through the geometry
void eventAction::BeginOfEventAction(const G4Event *)
{
  // Initialization per event.  We need to reset to the total energy
  // collected at the beginning of each event
  TotalEnergyDepTile1 = 0.;
  ParticleEnergy = 0.;
  ParticlePosition = G4ThreeVector(0., 0., 0.);
  MomentumDirection = 0.;
  InelasticCol = 0.;
  ElasticCol = 0.;
  IonizingCol = 0.;
  DT = 0.;
}

void eventAction::Secondaries(G4double NumSecondaries){
  // Tallies the number of secondaries generated in this event
  // into a larger list maintained by the RunAction.
  fRunAction->AddSecondaries(NumSecondaries);
}

void eventAction::PrintInfo(){
  // This is our output function, it returns Energy, Position, 
  // Direction (angle), and particle type
  if(dataOutputSwitch and (ParticleEnergy > 0))
    eventOutput << ParticleEnergy/keV << ";" << ParticlePosition/cm << ";" << MomentumDirection << ";" << ParticleTime/second << std::endl;
}

// Anything included in this function is performed at the very end of
// each event's lifetime.
void eventAction::EndOfEventAction(const G4Event *)
{
  // Each process type is given a number, for formatting in 
  // RunAction. This is for tallying types of interaction in
  // the target, seperate from Secondary generation.
  fRunAction->AddProcess(InelasticCol,1);
  fRunAction->AddProcess(ElasticCol,2);
  fRunAction->AddProcess(IonizingCol,3);
  fRunAction->AddProcess(DT,4);
  G4bool Proccess_out = false;
  // If the user has turned data output 'on', then do this!
  if(Proccess_out and (InelasticCol > 0))
    {
      processOutput << InelasticCol << ";" << IonizingCol << ";" << ElasticCol << std::endl;
    }
    
}
