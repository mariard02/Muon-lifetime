#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();

	// World material: air
	G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");

	G4MaterialPropertiesTable* MPT_air = new G4MaterialPropertiesTable();
	MPT_air->AddConstProperty("RINDEX", 1., true);
	worldMaterial->SetMaterialPropertiesTable(MPT_air);

	// Scintillator material: NaI Scintillator
	G4String symbol;
	G4double density, fractionmass;
	G4int ncomponents;

	// Adding elements to the material
	G4Element* Na = nist->FindOrBuildElement("Na");
	G4Element* I = nist->FindOrBuildElement("I");

	//G4Material* scintillator = new G4Material("NaI", density=3.67*g/cm3, ncomponents=2);
	//scintillator->AddElement(Na, fractionmass=0.5);
	//scintillator->AddElement(I, fractionmass=0.5);
	G4Material* scintillator = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	
	// scintillator properties
	std::vector<G4double> energy     = {2.034*eV, 3.*eV, 4.136*eV};
	std::vector<G4double> rindex     = {1.58, 1.58, 1.58};
	std::vector<G4double> absorption = {400.*cm, 400.*cm, 400.0*cm};
	
	G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
	
	// property independent of energy
	MPT->AddConstProperty("SCINTILLATIONYIELD", 9200./MeV);
	
	// properties that depend on energy
	MPT->AddProperty("RINDEX", energy, rindex);
	MPT->AddProperty("ABSLENGTH", energy, absorption);
	
	
	const G4int NUMENTRIES = 13;
	G4double Scnt_PP[NUMENTRIES] = { 2.39*eV, 2.43*eV, 2.48*eV, 2.53*eV, 2.58*eV, 2.64*eV, 2.70*eV, 2.76*eV, 2.82*eV, 2.89*eV, 2.95*eV, 3.03*eV, 3.10*eV};
	G4double Scnt_FAST[NUMENTRIES] = {0.02, 0.05, 0.10, 0.20, 0.35, 0.50, 0.65, 0.85, 1.00, 0.85, 0.65, 0.35, 0.10};
	//G4double Scnt_SLOW[NUMENTRIES] = { 0.000010, 0.000020, 0.000030, 0.004000,
	//0.008000, 0.005000, 0.020000, 0.001000,
	//0.000010 };
	MPT->AddProperty("SCINTILLATIONCOMPONENT1", Scnt_PP, Scnt_FAST, NUMENTRIES); // We only consider the fast component
	MPT->AddConstProperty("RESOLUTIONSCALE", 2.0);
	MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3.3*ns); 
	scintillator->SetMaterialPropertiesTable(MPT);

	// Target material: lead
	G4Material* Lead = nist->FindOrBuildMaterial("G4_Pb");

	// CREATE THE WORLD
	G4Box *solidworld = new G4Box("solidworld", 5.*m, 5.*m, 5.*m);
	G4LogicalVolume *logicworld = new G4LogicalVolume(solidworld, worldMaterial, "logicWorld");
	G4VPhysicalVolume *physworld  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicworld, "physworld", 0, false, 0., true);

	// CREATE THE SCINTILLATOR
	G4double ScinBoxSizeXY = 2. * m;
    G4double ScinBoxSizeZ  = 50.0 * cm;
    G4Box* solidScinBox = new G4Box("solidScinBox", 0.5*ScinBoxSizeXY, 0.5*ScinBoxSizeXY, 0.5*ScinBoxSizeZ);
    G4LogicalVolume *logicScin = new G4LogicalVolume(solidScinBox, scintillator, "logicScin");
    G4VPhysicalVolume *physScin  = new G4PVPlacement(0, G4ThreeVector(0., 0., 1.*m), logicScin, "physScin", logicworld, false, 0., true);

    // CREATE THE LEAD
    G4double LeadBoxSizeXY = 2. * m;
	G4double LeadBoxSizeZ = 3. * m; // Depth of the box in Z direction
	G4Box* solidLeadBox = new G4Box("solidLeadBox", LeadBoxSizeXY/2, LeadBoxSizeXY/2, LeadBoxSizeZ/2);
	G4LogicalVolume *logicLead = new G4LogicalVolume(solidLeadBox, Lead, "logicLead");
    G4VPhysicalVolume *physLead  = new G4PVPlacement(0, G4ThreeVector(0., 0., 3.*m), logicLead, "physLead", logicworld, false, 0., true);

    return physworld;
}
