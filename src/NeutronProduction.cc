#include "NeutronProduction.hh"

#include "G4SystemOfUnits.hh"
#include "G4TransportationProcessType.hh"
//#include "G4GeneralP"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4DynamicParticle.hh"
#include "G4Neutron.hh"
#include <iostream>

NeutronProduction::NeutronProduction(const G4String& processName, G4ProcessType aType)
    : G4VDiscreteProcess(processName, aType)
{
    SetProcessSubType(USER_SPECIAL_CUTS);}

NeutronProduction::~NeutronProduction()
{;}

G4VParticleChange* NeutronProduction::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
    aParticleChange.Initialize(aTrack);
    G4cout << "Process Called!!"<<G4endl;
    if(aParticleChange.GetNumberOfSecondaries() > 0){        
        G4cout <<"Neutrons generated"<<G4endl;
        // Get the current number of secondaries attached to the track
        G4int NumSecondaries = aParticleChange.GetNumberOfSecondaries();

        G4ThreeVector pDirection = aTrack.GetMomentumDirection();
        G4double pEnergy = aTrack.GetKineticEnergy();
        // Set up a vector to store our *new* secondaries in
        std::vector<G4DynamicParticle*> neutronVector;
        
        // Set up the information for our neutrons and add them to the neutron
        // vector
        for (G4int i = 0; i< NumSecondaries; i++){
            G4Track* sTrack = aParticleChange.GetSecondary(i);
            sTrack->SetTrackStatus(fStopAndKill);
            G4DynamicParticle* neutron = new G4DynamicParticle(G4Neutron::Neutron(), // The particle type
                                    pDirection, // the particle direction
                                    2.2*MeV); // the particle energy
            neutronVector.push_back(neutron);

            }
        aParticleChange.SetNumberOfSecondaries(NumSecondaries);
        G4DynamicParticle* aNeutron;
        for (G4int i=0; i<neutronVector.size();i++){
            aNeutron = neutronVector[i];
            if(aNeutron){
                aParticleChange.AddSecondary(aNeutron);
            }
        }

        return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);}
    else 
        return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);

}