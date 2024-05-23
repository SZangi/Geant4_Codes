#ifndef eventAction_hh
#define eventAction_hh 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "eventActionMessenger.hh"

#include <fstream>
#include <list>

class runAction;

// eventAction class handles information about entire events.  More
// specifically, it will receive energy deposited per step from
// steppingAction and recursively add it to get the total energy per
// event.  Also, eventAction is responsible for outputting information
// to data files, thus, it has user- controlled functions, which are
// set through eventActionMessenger, which control data output

class eventAction : public G4UserEventAction
{

public:
  eventAction(runAction* currentrun);
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

  void ProcessAdd(G4String PartProcess)
  { if(PartProcess == "biasWrapper(ionIoni)")
    {  IonizingCol ++;
      G4String processType = "Ionizing";
      }
    if(PartProcess == "biasWrapper(hadElastic)")
      {ElasticCol ++;
        G4String processType = "Elastic";}
    if (PartProcess == "biasWrapper(alphaInelastic)"){
      InelasticCol ++;
      G4String processType = "Inelastic";
    }
    if (PartProcess == "DT"){
      DT ++;
    }
        
  };

  void Secondaries (G4double NumSecondaires);

  void Detectors (G4double Energy);

  void GetEnergy(G4double PartEnergy)
  {
    ParticleEnergy = 0;
    ParticleEnergy += PartEnergy;
  }

  void GetMomentumDir(G4double PartMomentumDir)
  {
    MomentumDirection = PartMomentumDir;
  }

  void GetPosition(G4ThreeVector PartPosition)
  {
    ParticlePosition = PartPosition;
  }

  void GetParticleID(G4String PartID)
  {
    ParticleID = PartID;
  }

  void GetTrackWeight(G4double Weight)
  {
    ParticleWeight = Weight;
  }

  void GetTrackTime(G4double Time)
  {
    ParticleTime = Time;
  }

  void PrintInfo();

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

  G4double ParticleWeight;

  G4bool dataOutputSwitch;

  G4double MomentumDirection;

  G4ThreeVector ParticlePosition;

  G4String ParticleID;

  G4double InelasticCol;

  G4double IonizingCol;

  G4double ElasticCol;

  G4double DT;

  G4double ParticleTime;

  G4double Detect_Energy;
  
  eventActionMessenger *eventMessenger;
  
  std::ofstream eventOutput;

  std::ofstream processOutput;

  std::ofstream detectOutput;

  runAction* fRunAction = nullptr;
};
    
#endif
