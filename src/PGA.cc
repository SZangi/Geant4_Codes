#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "C13AngularData.hh"

#include "PGA.hh"

#include <iostream>

using namespace C13;
// PGA stands for Primary Generator Action.  This is the "source" of
// particles.  PGA creates point sources of particles.  A far more
// advanced particle source is the "G4GeneralParticleSource.cc/.hh"
// module that is included with Geant4.  See online documentation.

PGA::PGA() 
{
  // One 1 particle per event
  G4int n_particle = 1;

  // Create the particle gun to fire n_particles
  //particleSource = new G4GeneralParticleSource();
  particleSource = new G4ParticleGun();
  particleSource -> SetNumberOfParticles(n_particle);

  // Get the definition of a deuteron & proton from the internal particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* deuteron = particleTable -> FindParticle("deuteron");
  G4ParticleDefinition* proton = particleTable -> FindParticle("proton");
  G4ParticleDefinition* alpha = particleTable -> FindParticle("alpha");
  G4ParticleDefinition* helium3 = particleTable -> FindParticle("He3");
  G4ParticleDefinition* neutron = particleTable -> FindParticle("neutron");
  
  // Initialize the particle gun with some default values
  particleSource -> SetParticleDefinition(neutron);
  for (G4int i =0;i<E_alpha_size;i++){
    E_alpha_array[i] = E_in_vec[i];
  }

  alpha_generator = new G4RandGeneral(E_alpha_array,E_alpha_size);

  Hist_points.reserve(LegendreLength);
  // Set up the legendre histogram objects
  for (G4int i; i<LegendreEnergy.size();i++){
    G4double * Hist_point = new G4double[LegendreBins];
    for (G4int j; j<LegendreBins;j++){
      Hist_point[j] = LegendreHists[i][j];
    }
    Hist_points.emplace_back(Hist_point);
  }

  G4double X = 0.*m;
  G4double Y = 0.*m;
  G4double Z = 0.*cm;
  // Again, particle position can be set in the class definition like so, but
  // we set it from a macro for more flexibility.
  particleSource -> SetParticlePosition(G4ThreeVector(X,Y,Z));
}


PGA::~PGA()
{ delete particleSource;
  delete alpha_generator;
  delete action_generator;
  for (G4int i = 0; i<Hist_points.size();i++) delete Hist_points[i];
  delete E_alpha_array; }


void PGA::GeneratePrimaries(G4Event* anEvent)
{ // The plan: 
  // 1. Read in a histogram in constructor -> TH2 class
  // 2. Use the GetRandom2() method to sample energy and direction
  // 3. Set the particleSource direction and energy to the random values
  // each time
  E_incident = alpha_generator->shoot()*(E_max-E_min)+E_min;

  Particle_info = GenerateCosE(E_incident,m_a,m_b,m_c,m_d,Q);

  particleSource -> SetParticleEnergy(E_incident);

  //G4cout << E_incident << G4endl;

  //G4double angle = (G4UniformRand()-0.5)*2*pi;

  G4double radius = 1-pow(Particle_info[0],2);

  //G4double p_y = radius*sin(angle);
  G4double p_y = radius;
  //G4double p_z = radius*cos(angle);

  //G4cout<<Particle_info[0]<<G4endl;

  particleSource -> SetParticleMomentumDirection(G4ThreeVector(0,p_y,Particle_info[0]));

  particleSource -> GeneratePrimaryVertex(anEvent); } 


std::vector<G4double> PGA::GenerateCosE(G4double incident_E,G4double m_A, G4double m_B,G4double m_C,G4double m_D,G4double Q_value){
  G4int idx = 0;
  for (G4int i=0;i<LegendreEnergy.size();i++){
    if (incident_E > LegendreEnergy[i]*eV)
      idx = i;
    else
      break;
  }
  action_generator = new G4RandGeneral(Hist_points[idx],LegendreBins);
  G4double rand_num = action_generator->shoot(G4Random::getTheEngine());
  G4double part_cos = 2*rand_num-1;
  G4double part_E = CosToE(part_cos,m_A,m_B,m_C,m_D,incident_E,Q_value);
  
  delete action_generator;
  action_generator = nullptr;

  return {part_cos,part_E};
}

G4double PGA::CosToE(G4double cos, G4double m_A, G4double m_B,G4double m_C,G4double m_D,G4double E_in,G4double Q_value){
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


