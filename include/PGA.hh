#ifndef PGA_hh
#define PGA_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

// PGA class creates the particle gun at a specified location and
// in a specified direction with a specific particle. It is also 
// responsible for generating each event. 

class PGA : public G4VUserPrimaryGeneratorAction
{
public:
  PGA();
  ~PGA();

  void GeneratePrimaries(G4Event *);
  
private:
  G4GeneralParticleSource* particleSource;
};

#endif
