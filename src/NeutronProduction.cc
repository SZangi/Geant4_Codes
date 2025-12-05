#include "NeutronProduction.hh"
#include "C13AngularData.hh"
#include "B11AngularData.hh"

#include "G4SystemOfUnits.hh"
#include "G4TransportationProcessType.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4DynamicParticle.hh"
#include "G4Neutron.hh"
#include "G4StepPoint.hh"
#include "G4PhysicalConstants.hh"
#include "G4VProcess.hh"
#include "G4HadronicProcess.hh"
#include "G4BiasingProcessInterface.hh"

#include <iostream>

//using namespace C13;

NeutronProduction::NeutronProduction(const G4String& processName, G4ProcessType aType)
    : G4VDiscreteProcess(processName, aType)
{
    SetProcessSubType(NEUTRON_KILLER);
    
    // Set up the random generators for secondary production
    Hist_points.reserve(C13::LegendreLength);
    
    // Set up the legendre histogram objects
    for (G4int i=0; i<C13::LegendreEnergy.size();i++){
      
      // Create and fill a vector from the C13 data for each energy point
      G4double * Hist_point = new G4double[C13::LegendreBins];
      for (G4int j=0; j<C13::LegendreBins;j++){
        // Fill the vector
        Hist_point[j] = C13::LegendreHists[i][j];
      }
      // Add the pointer to the vector that we've filled to the vector of pointers for later histogramming
      Hist_points.emplace_back(Hist_point);
      
      // create a new generator object from the vector we just filled 
      // Is Hist_points vector necessary? To investigate later
      G4RandGeneral* leg_generator = new G4RandGeneral(Hist_point,C13::LegendreBins);
      
      // Add generator to list of generators
      Generators.push_back(leg_generator);
    }
    
    }

NeutronProduction::~NeutronProduction()
{
    // Free memory attached to the pointer vectors we filled earlier
    // Hopefully this prevents memory leak?
  for (G4int i = 0; i<Hist_points.size();i++) delete Hist_points[i];
  for (G4int i = 0; i<Generators.size();i++) delete Generators[i];
}

G4VParticleChange* NeutronProduction::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
    aParticleChange.Initialize(aTrack);
    if(aStep.GetNumberOfSecondariesInCurrentStep() > 0){
      G4bool aNeutron = false;
      for(G4int i = 0; i <aStep.GetNumberOfSecondariesInCurrentStep(); i++){
        aNeutron = aNeutron || (aStep.GetSecondaryInCurrentStep()[0][i]->GetParticleDefinition()->GetParticleName() == "neutron");
      }
      if (aNeutron){
        // Get the current number of secondaries attached to the track
        G4int NumSecondaries = aStep.GetNumberOfSecondariesInCurrentStep();
        G4StepPoint* post = aStep.GetPostStepPoint();
        G4VProcess* process   = const_cast<G4VProcess*>(post->GetProcessDefinedStep());
        G4BiasingProcessInterface* bprocess = dynamic_cast<G4BiasingProcessInterface*>(process);
        //G4cout<<bprocess->GetWrappedProcess()->GetProcessName()<<G4endl;
        G4HadronicProcess* hproc = NULL;
        if (bprocess->GetWrappedProcess()->GetProcessName()=="alphaInelastic")
          hproc = dynamic_cast<G4HadronicProcess*>(bprocess->GetWrappedProcess());
        const G4Nucleus* target = NULL;
        if (hproc) target = hproc->GetTargetNucleus();
        //G4cout <<aStep.GetPostStepPoint()->GetKineticEnergy() - aStep.GetPreStepPoint()->GetKineticEnergy() <<G4endl;
        G4int targetName = 0;  
        if (target) targetName = target->GetN_asInt();
        //if (targetName>0) G4cout<<targetName<<G4endl;
        if (targetName == 5){
          m_a = 10.012937;
          m_c = 13.005739;	
          Q = 1058.7*keV;
        }
        else if(targetName == 6){
          m_a = 11.009305;
          m_c = 14.003074;
          Q = 0.157*MeV;
        }
        else if(targetName == 7){
          m_a = 13.003355;
          m_c = 15.994915;
          Q = 2.216*MeV;
        }


        G4ThreeVector pDirection = aTrack.GetMomentumDirection();
        
        G4double pEnergy = aStep.GetPreStepPoint()->GetKineticEnergy();

        // Set up a vector to store our *new* secondaries in
        std::vector<G4DynamicParticle*> neutronVector;
        const std::vector<const G4Track*>* secondary_vector= aStep.GetSecondaryInCurrentStep();
        auto aTrack_vector = secondary_vector[0];
        
        // Set up the information for our neutrons and add them to the neutron
        // vector
        for (G4int i = 0; i< NumSecondaries; i++){
            // commit a crime so that we can get rid of the neutron!
            // const_cast is reallly not recommended(weird things happen when you call it), 
            // so if I can find a way to do this in the future that doesn't require this,
            // I would really like to
            G4Track* sTrack = const_cast<G4Track*> (aTrack_vector[i]); 
            if(sTrack->GetParticleDefinition()->GetParticleName() =="neutron"){
                sTrack->SetTrackStatus(fStopAndKill);
                //G4cout<<pEnergy<<"MeV"<<G4endl;

                // Use helper function to generate energy and primary direction (theta)
                Particle_info = GenerateCosE(pEnergy,m_a,m_b,m_c,m_d,Q);
                // Pick a secondary angle (phi)
                G4double angle = (G4UniformRand()*2)-1;
                G4double sine = 0;
                if(CLHEP::RandBit::shootBit())
                  sine = -1*sqrt(1-pow(angle,2));
                else
                  sine = sqrt(1-pow(angle,2));
                // Normalize the momentum vector to 1
                G4double radius = sqrt(1-pow(Particle_info[0],2));
                // Calculate the remaining two directions 
                G4double p_y = radius*sine;
                G4double p_z = radius*angle;

                //G4double vector_length = pow(p_y,2)+pow(p_z,2)+pow(Particle_info[0],2);
                /*Use the CLHEP rotateUz function to correct for difference between the lab
                  reference frame and the COM reference frame*/

                G4ThreeVector sDirection = G4ThreeVector(p_z,p_y,Particle_info[0]);
                G4ThreeVector fDirection = sDirection.rotateUz(pDirection);

                G4DynamicParticle* neutron = new G4DynamicParticle(G4Neutron::Neutron(), // The particle type
                                        fDirection, // the particle direction
                                        Particle_info[1]); // the particle energy
                neutronVector.push_back(neutron);
            }

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
    else 
        return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);

}

std::vector<G4double> NeutronProduction::GenerateCosE(G4double incident_E,G4double m_A, G4double m_B,G4double m_C,G4double m_D,G4double Q_value){
  G4int idx = 0;
  for (G4int i=0;i<C13::LegendreEnergy.size();i++){
    if (incident_E > C13::LegendreEnergy[i]*eV)
      idx = i;
    else
      break;
  }

  G4double rand_num = Generators[idx]->shoot(G4Random::getTheEngine());
  G4double part_cos = (2*rand_num-1);
  G4double part_E = CosToE(part_cos,m_A,m_B,m_C,m_D,incident_E,Q_value);
  
  return {part_cos,part_E};
}

G4double NeutronProduction::CosToE(G4double cos, G4double m_A, G4double m_B,G4double m_C,G4double m_D,G4double E_in,G4double Q_value){
    A_masses = (m_B*m_C)/((m_B+m_A)*(m_D+m_C));
    A_energy = (E_in/(E_in+Q_value));
    A = A_masses*A_energy;
    B_masses = m_B*m_D/((m_B+m_A)*(m_D+m_C));
    B_energy = (E_in/(E_in+Q_value));
    B = B_masses*B_energy;
    C_masses = (m_D*m_A)/((m_B+m_A)*(m_D+m_C));
    C_energy = (1+m_B*Q_value/(m_A*(Q_value+E_in)));
    C = C_energy*C_masses;
    D_masses = m_A*m_C/((m_B+m_A)*(m_D+m_C));
    D_energy = (1+m_B/m_A*Q_value/(E_in+Q_value));
    D = D_masses*D_energy;

    G4double step1 = sqrt(((D/B)-pow(sin(acos(cos)),2)));
    G4double step2 = cos + step1;
    G4double step3 = B*(E_in+Q_value)*pow(step2,2);
    G4double En_temp = step3;

  return En_temp;
}