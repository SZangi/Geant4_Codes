#ifndef trackAction_hh
#define trackAction_hh 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"


#include <fstream>

// eventAction class handles information about entire events.  More
// specifically, it will receive energy deposited per step from
// steppingAction and recursively add it to get the total energy per
// event.  Also, eventAction is responsible for outputting information
// to data files, thus, it has user- controlled functions, which are
// set through eventActionMessenger, which control data output

class trackingAction : public G4UserTrackingAction
{

public:
  eventAction();
  ~eventAction();

  // These are virtual functions of G4UserEventAction that the user
  // can redefine to perform his or her heart's desire.  They always
  // accept a G4Event * as an argument
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(const G4Event *);
  
  // This is a user-dfined function to add the energy deposited at
  // each step to a variable that holds total energy deposited per
  // event.  It is called by "steppingAction.cc" every time there is
  // energy deposited on a step
  void AddEnergyDep(G4double EDepTile1)  
  {TotalEnergyDepTile1 += EDepTile1;
  };

  void GetEnergy(G4double PartEnergy)
  {
    ParticleEnergy += PartEnergy;
  }

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