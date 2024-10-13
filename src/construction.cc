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

	// Scintillator material: NaI Scintillator
	G4String symbol;
	G4double density, fractionmass;
	G4int ncomponents;

	// Adding elements to the material
	G4Element* Na = nist->FindOrBuildElement("Na");
	G4Element* I = nist->FindOrBuildElement("I");

	G4Material* scintillator = new G4Material("NaI", density=3.67*g/cm3, ncomponents=2);
	scintillator->AddElement(Na, fractionmass=0.5);
	scintillator->AddElement(I, fractionmass=0.5);
	
	// scintillator properties
	std::vector<G4double> energy     = {2.034*eV, 3.*eV, 4.136*eV};
	std::vector<G4double> rindex     = {1.3435, 1.351, 1.3608};
	std::vector<G4double> absorption = {344.8*cm, 850.*cm, 1450.0*cm};
	
	G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
	
	// property independent of energy
	MPT->AddConstProperty("SCINTILLATIONYIELD", 100./MeV);
	
	// properties that depend on energy
	MPT->AddProperty("RINDEX", energy, rindex);
	MPT->AddProperty("ABSLENGTH", energy, absorption);
	
	
	const G4int NUMENTRIES = 9;
	G4double Scnt_PP[NUMENTRIES] = { 6.6*eV, 6.7*eV, 6.8*eV, 6.9*eV,
	7.0*eV, 7.1*eV, 7.2*eV, 7.3*eV, 7.4*eV };
	G4double Scnt_FAST[NUMENTRIES] = { 0.000134, 0.004432, 0.053991, 0.241971,
	0.398942, 0.000134, 0.004432, 0.053991,
	0.241971 };
	G4double Scnt_SLOW[NUMENTRIES] = { 0.000010, 0.000020, 0.000030, 0.004000,
	0.008000, 0.005000, 0.020000, 0.001000,
	0.000010 };
	MPT->AddProperty("SCINTILLATIONCOMPONENT1", Scnt_PP, Scnt_FAST, NUMENTRIES);
	MPT->AddProperty("SCINTILLATIONCOMPONENT2", Scnt_PP, Scnt_SLOW, NUMENTRIES);
	MPT->AddConstProperty("RESOLUTIONSCALE", 2.0);
	MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1.*ns);
	MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10.*ns);
	MPT->AddConstProperty("SCINTILLATIONYIELD1", 0.8);
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
