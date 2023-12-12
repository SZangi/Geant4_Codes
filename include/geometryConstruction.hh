#ifndef geometryConstruction_hh
#define geometryConstruction_hh 1

#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include <G4Material.hh>

// Define the geometryConstruction class where all the physical
// aspects of the of the simulation will be built: geometry,
// materials, physical placement, and visualization attributes.
class geometryConstruction : public G4VUserDetectorConstruction
{
  
public:
  geometryConstruction(); // Constructor
  ~geometryConstruction(); // Destructor
  
  // Main function
  G4VPhysicalVolume *Construct();
  virtual void     ConstructSDandField();
  
  // Define member functions to create materials
  G4Material *mixVacuum();
  G4Material *mixScintillator();
  G4Material *mixAir();
  G4Material *mixTritium();
  G4Material *Boron();
  G4Material *Hydrogen();
  G4Material *mixHelium3();
  G4Material *mixLithium();
  G4Material *IsoCarbon13();
};

#endif
