#ifndef steppingAction_hh
#define steppingAction_hh 1

#include "G4UserSteppingAction.hh"

class eventAction;

// steppingAction class collects information about the particles
// at each step.  More specifically, steppingAction will collect
// the energy deposited by the particle for each step and then
// pass it to eventAction class, which accumulates the TOTAL
// energy deposited for each EVENT.
class steppingAction : public G4UserSteppingAction
{

public:
  steppingAction(eventAction *);
  ~steppingAction();
  
  void UserSteppingAction(const G4Step *);
  
private:
  eventAction *evtAction;
  G4double PartMomentumA;
  G4double alphaE=0;
};

#endif
