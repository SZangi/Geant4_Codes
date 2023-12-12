#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"
#include "GB01BOptrChangeCrossSection.hh"
#include "G4LogicalVolumeStore.hh"

#include "geometryConstruction.hh"

geometryConstruction::geometryConstruction()
{;}

geometryConstruction::~geometryConstruction()
{;}

// This is the main member function for the geometryConstruction
// class.  It is called by the runManager automatically when the new
// object of type geometryConstruction is initialized and passed to
// the runManager in "scintTest.cc"
G4VPhysicalVolume *geometryConstruction::Construct()
{
  // Call class member functions to mix desired materials.  Functions
  // will return pointer of type G4Material
  G4Material *vacuum = mixVacuum();
  G4Material *scintillator = mixScintillator();
  G4Material *air = mixAir();
  G4Material *tritium = mixTritium();
  G4Material *boron = Boron();
  G4Material *hydrogen = Hydrogen();
  G4Material *helium3 = mixHelium3();
  G4Material *carbon13 = IsoCarbon13();
//  G4Material *lithium = mixLithium();
    
  // Create the top-level volume in the geometry hierarchy, known as
  // the world volume.  Note that the world volume is almost always a
  // G4Box.  Let's call is the "lab" and make it a long rectangle of
  // the following dimensions....
  G4double labX = 10*cm;
  G4double labY = 10*cm;
  G4double labZ = 20*cm;
  
  // Create a "solid" volume pointer of type G4Box.  Solid volumes in
  // Geant4 define the shape of a given volume.
  G4Box *lab_s = new G4Box("lab_s",labX,labY,labZ);

  // Create a "logical" volume pointer of type G4LogicalVolume and
  // attach it to the corresponding solid volume.  LV's in Geant4
  // control the physical attributes of the volume, such as what
  // material it's made out of, E and B fields, etc.
  G4LogicalVolume *lab_l = new G4LogicalVolume(lab_s, // solid to attach to
					       vacuum, // material
					       "lab_l", // unique name of logical
					       0,
					       0,
					       0);

  // Create the "physical" volume pointer of type G4PVPlacement and
  // attach it to the corresponding LV.  The PV controls physical
  // placement, such as position and rotation.  Note that the world
  // volume is always unrotated and unpositioned
  G4VPhysicalVolume *lab_p = new G4PVPlacement(0, // No rotation
					       G4ThreeVector(), // No Position
					       lab_l, // logical to attach to
					       "lab", // name of physical
					       0,
					       false,
					       0);


  // Create the scoring shell. Should be filled with vacuum or similar
  // material to the lab frame, and should fully contain the target
  // tile.

  G4double shellR_max = 5. *cm;
  G4double shellR_min = 4.9 *cm;

  G4Sphere *scoring_shell = 
    new G4Sphere("scoring_shell",
                shellR_min,     // Shell inner radius
                shellR_max,     // Shell outer radius
                0,              // Starting phi angle
                2*pi,           // Length of segment in phi
                0,              // Starting theta angle
                pi);            // length of segment in theta

  G4LogicalVolume *score_l = 
    new G4LogicalVolume(scoring_shell,
                        vacuum,
                        "score_l",
                        0,
                        0,
                        0);
  
    G4double posX = 0.*m; // position the scoring shell at the center, 10 cm off the front of the world
    G4double posY = 0.*m;
    G4double posZ = -10*cm;

  G4VPhysicalVolume *score_p =
    new G4PVPlacement(new G4RotationMatrix(),
                      G4ThreeVector(posX, posY, posZ),
                      score_l,
                      "score_p",
                      lab_l,
                      false,
                      0);

  // Create the necessary volumes for the scintillator pieces, which
  // are modelled as thin, square tiles

  G4double scintBoxX = 3.*cm; // remember all of these are half widths
  G4double scintBoxY = 3.*cm;
  G4double scintBoxZ = 0.5*um;

  // Create solid volume representing the shape of the tiles
  G4Box *scint_s = new G4Box("scint_s",scintBoxX,scintBoxY,scintBoxZ);

  // 2 different logical volumes created to two different colors 
  // can be applied...remember, its all about the style points.
  G4LogicalVolume *scint_l1 = new G4LogicalVolume(scint_s,
						  tritium,
						  "scint_l1",
						  0,
						  0,
						  0);

  // We will place the 2 pieces inside the world volume.  Thus, the
  // world volume is the "mother" volume and the scintillators are
  // "daughter" volumes of the world volume.
  G4VPhysicalVolume *scint_p1 = 
    new G4PVPlacement(new G4RotationMatrix(), // No Rotation
		      G4ThreeVector(posX,posY,posZ), // Place at the center of the scoring volume
		      scint_l1,
		      "scint_p1",
		      lab_l, // keep the lab as the mother volume, as we made the scoring volume a shell
		      false,
		      0);

  // We'd like to seperate the tiles.  In Geant4, all volumes are
  // placed based on their exact center, so we need to account for this.
 // G4double space = 4*cm;

//  G4VPhysicalVolume *scint_p2 = 
 //   new G4PVPlacement(new G4RotationMatrix(),
//		      G4ThreeVector(posX,posY,posZ-space),
//		      scint_l2,
//		      "scint_p2",
//		      lab_l, // the lab is the mother volume
//		      false,
//		      0);

  // To visualize our geometry, we need to attach visAttributes to
  // each logical volume.
  G4VisAttributes* LabVisAtt = new G4VisAttributes(G4Colour());
  LabVisAtt -> SetVisibility(1);
  lab_l -> SetVisAttributes(LabVisAtt);
  
  G4VisAttributes *ScintBox1VisAtt = 
    new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));
  ScintBox1VisAtt -> SetForceSolid(1);
  scint_l1 -> SetVisAttributes(ScintBox1VisAtt);
  
  G4VisAttributes *ScoringShellVisAtt = 
    new G4VisAttributes(G4Colour());
  ScoringShellVisAtt -> SetVisibility(1);
  score_l -> SetVisAttributes(ScoringShellVisAtt);

//  G4VisAttributes *ScintBox2VisAtt = 
//   new G4VisAttributes(G4Colour(1.0,1.0,0.0,1.0));
//ScintBox2VisAtt -> SetForceSolid(1);
// scint_l2 -> SetVisAttributes(ScintBox2VisAtt);
  
  // Return a pointer to the world volume
  return lab_p;
}

void geometryConstruction::ConstructSDandField()
{
  // -- Fetch volume for biasing:
  G4LogicalVolume* logicTest = G4LogicalVolumeStore::GetInstance()->GetVolume("scint_l1");
  
  // ----------------------------------------------
  // -- operator creation and attachment to volume:
  // ----------------------------------------------
  GB01BOptrChangeCrossSection* testMany = 
    new GB01BOptrChangeCrossSection("deuteron");
  testMany->AttachTo(logicTest);
  G4cout << " Attaching biasing operator " << testMany->GetName()
         << " to logical volume " << logicTest->GetName()
         << G4endl;
}


G4Material *geometryConstruction::mixVacuum()
{
  // Materials can be created by defining the atomic mass, proton
  // number, and density.  There are many ways to define elements,
  // isotopes, and materials.  The reader should consult the Geant4
  // user's manual.

  G4double a = 1.01*g/mole;
  G4double z = 1.;
  G4double density = universe_mean_density; // A Geant4 constant

  G4Material* vacuum = new G4Material("Galactic", 
				      z=1., 
				      a=1.01*g/mole,
				      density= universe_mean_density,
				      kStateGas, 
				      3.e-18*pascal, 
				      2.73*kelvin);
  
  return vacuum;
}


G4Material *geometryConstruction::mixScintillator()
{
  // Saint Gobain BC404 specifications obtained from:
  // http://www.detectors.saint-gobain.com/ -> search for "BC404"
  G4double a;
  G4double z;
  G4double density;
  G4int nComp;
  G4int Catoms;
  G4int Hatoms;
  
  a = 12.01*g/mole;
  z = 6.;
  G4Element *C = new G4Element("carbon", "C", z,a);

  a = 1.01*g/mole;
  z = 1.;
  G4Element *H = new G4Element("hydrogen", "H", z,a);
  
  // Another way to create materials...specifying the number of atoms
  // of each element...
  density = 1.032*g/cm3;
  nComp = 2;
  Catoms = 474;
  Hatoms = 521;
  G4Material *scintillator = new G4Material("scintillator", density, nComp);
  scintillator -> AddElement(C, Catoms);
  scintillator -> AddElement(H, Hatoms);

  return scintillator;
}

  
G4Material *geometryConstruction::mixAir()
{
  // Air
  G4double a;
  G4double z;
  G4double density;
  G4int nel;
  
  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
 
  // Yet another way to create materials...specifying the percent (by
  // mass) of each element...
  G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);
  
  return Air;
}

G4Material *geometryConstruction::mixTritium() {
  // Tritium
  G4double a;
  G4double z;
  G4double n;
  G4double density = 0.257*g/cm3;
  G4int nComp;

  G4Isotope *T = new G4Isotope("Tritium", z= 1, n= 3, a= 3.014 *g/mole);
  G4Element *PureT = new G4Element("Pure Tritium", "T", nComp = 1);
  PureT -> AddIsotope(T, 100. *perCent);
  G4Material *Tritium = new G4Material("Tritium", density, nComp);
  Tritium -> AddElement(PureT, 100. *perCent);

  return Tritium;
}

G4Material *geometryConstruction::Boron() {
  // Boron
  G4double a;
  G4double z;
  G4double n;
  G4double density = 2.46*g/cm3;
  G4int nComp;

  G4Isotope *B11 = new G4Isotope("Boron11", z= 5, n= 11, a= 11.009305 *g/mole);
  G4Element *PureB11 = new G4Element("Elemental Boron 11", "B11", nComp = 1);
  PureB11 -> AddIsotope(B11, 100. *perCent);
  G4Material *Boron11 = new G4Material("MixBoron11", density, nComp);
  Boron11 -> AddElement(PureB11, 100. *perCent);

  return Boron11;
}

G4Material *geometryConstruction::Hydrogen() {
  // Regular Hydrogen
  G4double a;
  G4double z;
  G4double n;
  G4double density = 0.0763*g/cm3;
  G4int nComp;

  G4Isotope *H1 = new G4Isotope("H1", z= 1, n= 1, a= 1.004 *g/mole);
  G4Element *PureH1 = new G4Element("Elemental Hydrogen", "PureH1", nComp = 1);
  PureH1 -> AddIsotope(H1, 100. *perCent);
  G4Material *Hydrogen1 = new G4Material("Hydrogen1", density, nComp);
  Hydrogen1 -> AddElement(PureH1, 100. *perCent);

  return Hydrogen1;
}

G4Material *geometryConstruction::mixHelium3() {
  // Helium 3
  G4double a;
  G4double z;
  G4double n;
  G4double density = 0.000689*g/cm3;
  G4int nComp;

  G4Isotope *He3 = new G4Isotope("IsoHe3", z= 2, n= 3, a= 3.016 *g/mole);
  G4Element *PureHe3 = new G4Element("Elemental He3", "PureHe3", nComp = 1);
  PureHe3 -> AddIsotope(He3, 100. *perCent);
  G4Material *Helium3 = new G4Material("Helium3", density, nComp);
  Helium3 -> AddElement(PureHe3, 100. *perCent);

  return Helium3;
}

G4Material *geometryConstruction::mixLithium() {
  // Natural Lithium
  G4double a;
  G4double z;
  G4double n;
  G4double density = 0.597*g/cm3;
  G4int nComp;

  G4Element *Lithium = new G4Element("Lithium", "Li", z=3., a = 6.491 *g/mole);
  G4Material *NatLithium = new G4Material("NatLi", density, nComp);
  NatLithium -> AddElement(Lithium, 100. *perCent);

  return NatLithium;
}

G4Material *geometryConstruction::IsoCarbon13() {
  // Carbon13
  G4double a;
  G4double z;
  G4double n;
  G4double density = 1.604*g/cm3;
  G4int nComp;

  G4Isotope *C13 = new G4Isotope("Carbon-13", z= 6, n= 13, a= 13.0035 *g/mole);
  G4Element *PureC13 = new G4Element("Pure Carbon13", "C", nComp = 1);
  PureC13 -> AddIsotope(C13, 100. *perCent);
  G4Material *Carbon13 = new G4Material("Carbon13", density, nComp);
  Carbon13 -> AddElement(PureC13, 100. *perCent);

  return Carbon13;
}