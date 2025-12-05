#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"

#include "steppingAction.hh"
#include "eventAction.hh"

#include <iostream>

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
  G4double PartWeight = currentTrack -> GetWeight();

  // If in the scoring volume & a neutron, get a bunch of info about the particle...
  G4double PartNrg = 0.;
  G4ThreeVector PartMomentumD;
  G4ThreeVector PartPosition;
  G4String processName;
  G4ThreeVector BeamDir = G4ThreeVector(0.,0.0,1);
  G4double Secondaries = 0.;
  G4double currentTime = 0.;
  G4bool FirstVolStep;
  alphaE = 0;
  FirstVolStep = currentStep -> IsFirstStepInVolume();
  Secondaries = currentStep -> GetNumberOfSecondariesInCurrentStep();
    if((currentVolumeName == "score_p") and FirstVolStep){ //"score_p" for shell, "scint_p1" for target, also checking if first step for emission info
        PartNrg = 0.;       
        PartNrg += currentTrack -> GetKineticEnergy();
        if(currentParticleType == "alpha"){
          alphaE = PartNrg;
          evtAction -> GetTrackTime(alphaE);
        }
        if (currentParticleType == "neutron" && PartNrg > 0.){ // swap this out for the incident particle name
          // get energy of particle
          PartMomentumD = currentTrack -> GetMomentumDirection();
          PartMomentumA = PartMomentumD.angle(BeamDir);
          PartPosition = currentTrack -> GetPosition();
          currentTime = currentTrack -> GetGlobalTime();
          evtAction -> GetEnergy(PartNrg);
          evtAction -> GetMomentumDir(PartMomentumA);
          evtAction -> GetPosition(PartPosition);
          evtAction -> GetTrackWeight(PartWeight);
          evtAction -> PrintInfo();
      }
    }
    // If in the detector volume, dump energy
  
    else if((currentVolumeName == "detect_p" and currentParticleType == "neutron") & FirstVolStep){
      PartNrg = 0.;
      PartNrg += currentTrack -> GetKineticEnergy();
      evtAction -> Detectors(PartNrg,PartWeight);
      evtAction -> PrintInfo();
    }
    

      

      

  // If in the target and a deuteron, get us a bunch of information about the secondaries...
  //if(currentVolumeName == "scint_p1")
  //  if (currentParticleType == "He3")
  //    processName = postStepPoint -> GetProcessDefinedStep()->GetProcessName();
  //       if (processName != "CoupledTransportation" and processName.length() > 0)
          // // This is a rudamentary way of tallying specifically DT, by just tallying
          // // every inelastic reaction that produces two secondaries.
            //if (processName == "biasWrapper(dInelastic)")
            //  evtAction -> ProcessAdd("Inelastic");
            //else
            // Secondaries = currentStep -> GetNumberOfSecondariesInCurrentStep();
            // evtAction -> Secondaries(Secondaries);
          // // Pass that information to the eventAction, which will tally it pass it to 
          // // the run action in the correct format.
  //        evtAction -> ProcessAdd(processName);
    
  


  // Once we have the information we want about the generated particle, pass to 
  // eventAction, which stores that information and dumps it to the output file.  


  //if (Secondaries > 0){
  //  G4cout << Secondaries << G4endl;
  //}
  //G4bool transmitted = (preStepPoint->GetStepStatus() <= fGeomBoundary);
}


