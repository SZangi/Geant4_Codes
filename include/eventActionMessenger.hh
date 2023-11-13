#ifndef eventActionMessenger_hh
#define eventActionMessenger_hh 1

#include "G4UImessenger.hh"

class eventAction;
class G4UIdirectory;
class G4UIcmdWithAString;

// eventActionMessenger class allows the user to interface with
// eventAction class.  See 'eventAction.hh' for more details
class eventActionMessenger: public G4UImessenger
{

public:
  eventActionMessenger(eventAction *);
  ~eventActionMessenger();

  void SetNewValue(G4UIcommand *, G4String);

private:
  eventAction *EA;
  G4UIdirectory *outputDir;
  G4UIcmdWithAString *fileCommand;
  G4UIcmdWithAString *dataCommand;
};

#endif
