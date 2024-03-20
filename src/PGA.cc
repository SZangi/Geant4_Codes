#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "PGA.hh"

// PGA stands for Primary Generator Action.  This is the "source" of
// particles.  PGA creates point sources of particles.  A far more
// advanced particle source is the "G4GeneralParticleSource.cc/.hh"
// module that is included with Geant4.  See online documentation.

PGA::PGA() 
{
  // One 1 particle per event
  G4int n_particle = 1;

  // Create the particle gun to fire n_particles
  particleSource = new G4GeneralParticleSource();
  particleSource -> SetNumberOfParticles(n_particle);

  // Get the definition of a deuteron & proton from the internal particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* deuteron = particleTable -> FindParticle("deuteron");
  G4ParticleDefinition* proton = particleTable -> FindParticle("proton");
  G4ParticleDefinition* alpha = particleTable -> FindParticle("alpha");
  G4ParticleDefinition* helium3 = particleTable -> FindParticle("He3");
  G4ParticleDefinition* neutron = particleTable -> FindParticle("neutron");
  
  // Initialize the particle gun with some default values
  particleSource -> SetParticleDefinition(alpha);

  G4double X = 0.*m;
  G4double Y = 0.*m;
  G4double Z = 13.*cm;
  // Again, particle position can be set in the class definition like so, but
  // we set it from a macro for more flexibility.
  particleSource -> SetParticlePosition(G4ThreeVector(X,Y,Z));
}


PGA::~PGA()
{ delete particleSource; }


void PGA::GeneratePrimaries(G4Event* anEvent)
{ particleSource -> GeneratePrimaryVertex(anEvent); } 

