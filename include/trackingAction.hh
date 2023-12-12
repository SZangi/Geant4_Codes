#ifndef trackingAction_h
#define trackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "eventAction.hh"
#include "eventActionMessenger.hh"

#include <fstream>

// trackingAction should just accumulate all the information we care about 
// for one track. In our case, this grabs a neutrons energy, positon,
// and momentum as it passes into the recording shell. 

class trackingAction : public G4UserTrackingAction
{

public:
  trackingAction();
  ~trackingAction();

  void BeginOfTrackingAction(const G4Track *);
  void EndOfTrackingAction(const G4Track *);
  
  // This is a user-dfined function to add the energy deposited at
  // each step to a variable that holds total energy deposited per
  // event.  It is called by "steppingAction.cc" every time there is
  // energy deposited on a step
    void GetEnergy(G4double PartEnergy)
  {
    ParticleEnergy += PartEnergy;
  }

    void GetMomentum();

  // The following two functions are called from eventActionMessenger
  // at runtime when the user desires to change something....
  void SetDataOutput(G4String onOff)
  { if(onOff == "on") dataOutputSwitch = true;
    if(onOff == "off") dataOutputSwitch = false;};
  
  void SetOutputFileName(G4String fName)
  {if(eventOutput.is_open()) eventOutput.close();
    eventOutput.open(fName,std::ofstream::trunc);};
  
private:
  G4double TotalEnergyDepTile1;
  
  G4double ParticleEnergy;

  G4bool dataOutputSwitch;
  
  eventActionMessenger *eventMessenger;
  
  std::ofstream eventOutput;
};
    
#endif