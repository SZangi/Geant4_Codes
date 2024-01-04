#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"

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
  //G4StepPoint *postStepPoint = currentStep -> GetPostStepPoint();
  //G4StepPoint *preStepPoint = currentStep -> GetPreStepPoint();
  

  // Determine what volume the particle is currently in
  G4String currentVolumeName = currentTrack -> GetVolume() -> GetName();
  G4String currentParticleType = currentTrack -> GetParticleDefinition() -> GetParticleName();
  
  // If in the scoring volume & a neutron, get a bunch of info about the particle...
  G4double PartNrg = 0.;
  G4ThreeVector PartMomentumD;
  G4ThreeVector PartPosition;
  G4String processName;
  G4ThreeVector BeamDir = G4ThreeVector(0.,0.,-1.);
  //G4double Secondaries = 0.;
  //G4double PreStepNrg = 0.;
  //Secondaries = currentStep -> GetNumberOfSecondariesInCurrentStep();
  //PreStepNrg = preStepPoint -> GetKineticEnergy();
  if (currentParticleType == "neutron") // swap this out for the incident particle name
    if(currentVolumeName == "score_p") //"score_p" for shell, "scint_p1" for target
      // get energy of particle
      PartNrg = currentTrack -> GetKineticEnergy();
      PartMomentumD = currentTrack -> GetMomentumDirection();
      PartMomentumA = PartMomentumD.angle(BeamDir);
      PartPosition = currentTrack -> GetPosition();
      

  // If in scintillator tile 2, get the energy deposited...
  //G4double EDepTile2 = 0.;
 // if(currentVolumeName == "scint_p1")
   // if (currentParticleType == "deuteron")
     // processName = postStepPoint -> GetProcessDefinedStep()->GetProcessName();
        // if (processName != "CoupledTransportation" and processName.length() > 0)
           // if (Secondaries == 2 and processName == "biasWrapper(dInelastic)")
             // evtAction -> ProcessAdd("DT");
            //else
            //  evtAction -> Secondaries(PreStepNrg);
          //evtAction -> ProcessAdd(processName);
    
  
  // Once we have the information we want about the generated particle, pass to 
  // eventAction, which stores that information and dumps it to the output file.  
  evtAction -> GetEnergy(PartNrg);
  evtAction -> GetMomentumDir(PartMomentumA);
  evtAction -> GetPosition(PartPosition);
  evtAction -> GetParticleID(currentParticleType);

  //if (Secondaries > 0){
  //  G4cout << Secondaries << G4endl;
  //}

  if (currentParticleType == "neutron") // this should be the incident particle name, or the particle of interest
    if(currentVolumeName == "score_p") //"score_p" for shell, "scint_p1" for target
      evtAction -> PrintInfo();
  
}

