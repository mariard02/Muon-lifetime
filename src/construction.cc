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
	MPT->AddProperty("SCINTILLATIONCOMPONENT1", Scnt_PP, Scnt_FAST, NUMENTRIES); // We only consider the fast component
	MPT->AddConstProperty("RESOLUTIONSCALE", 2.0);
	MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 3.3*ns); 
	scintillator->SetMaterialPropertiesTable(MPT);

	// Target material: lead
	G4Material* Lead = nist->FindOrBuildMaterial("G4_Pb");

	// Material for the optical fiber
	G4Material* polystyrene_fibre = nist->FindOrBuildMaterial("G4_POLYSTIRENE_FIBRE");
	polystyrene_fibre = new G4Material("polystyrene_fibre", density = 1.200*g/cm3, ncomponents = 2);
	G4Element* H = nist->FindOrBuildElement("H");
	G4Element* C = nist->FindOrBuildElement("C");
	polystyrene_fibre->AddElement(H, 1);
	polystyrene_fibre->AddElement(C, 2);


	G4double photonEnergy_wls[] =
	{2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
	2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
	2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
	2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
	2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
	2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
	2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
	3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
	3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
	3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

	const G4int nEntries_wls = sizeof(photonEnergy_wls)/sizeof(G4double);

	G4double refractiveIndexWLSfiber[] =
	{ 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
	1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
	1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
	1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
	1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60};

	assert(sizeof(refractiveIndexWLSfiber) == sizeof(photonEnergy_wls));

	G4double absWLSfiber[] =
	{5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
	5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
	5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,1.10*m,
	1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.10*m, 1.*mm, 1.*mm, 1.*mm, 1.*mm,
	1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm};

	assert(sizeof(absWLSfiber) == sizeof(photonEnergy_wls));

	G4double emissionFib[] =
	{0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
	3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
	12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
	15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
	0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

	assert(sizeof(emissionFib) == sizeof(photonEnergy_wls));

	// Add entries into properties table
	G4MaterialPropertiesTable* mptWLSfiber = new G4MaterialPropertiesTable();
	mptWLSfiber->
	   AddProperty("RINDEX",photonEnergy_wls,refractiveIndexWLSfiber,nEntries_wls);
	// mptWLSfiber->AddProperty("ABSLENGTH",photonEnergy_wls,absWLSfiber,nEntries_wls);
	mptWLSfiber->AddProperty("WLSABSLENGTH",photonEnergy_wls,absWLSfiber,nEntries_wls);
	mptWLSfiber->AddProperty("WLSCOMPONENT",photonEnergy_wls,emissionFib,nEntries_wls);
	mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);

	polystyrene_fibre->SetMaterialPropertiesTable(mptWLSfiber);

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

    // Optical surface: mirror between the scintillator and the air.
    G4OpticalSurface* opAirScintillator = new G4OpticalSurface("AirScintillator");
    opAirScintillator -> SetType(dielectric_dielectric);
    opAirScintillator -> SetFinish(ground);
    opAirScintillator -> SetModel(glisur);
    opAirScintillator -> SetPolish(0.8);

    G4MaterialPropertiesTable* OpSurfaceProperty = new G4MaterialPropertiesTable();

    OpSurfaceProperty -> AddConstProperty("REFLECTIVITY", 0.8, true);

    opAirScintillator -> SetMaterialPropertiesTable(OpSurfaceProperty);

    G4LogicalBorderSurface* AirScintillator = new G4LogicalBorderSurface("AirScintillator", physworld, physScin ,opAirScintillator);

    // CREATE THE LEAD
    G4double LeadBoxSizeXY = 2. * m;
	G4double LeadBoxSizeZ = 3. * m; // Depth of the box in Z direction
	G4Box* solidLeadBox = new G4Box("solidLeadBox", LeadBoxSizeXY/2, LeadBoxSizeXY/2, LeadBoxSizeZ/2);
	G4LogicalVolume *logicLead = new G4LogicalVolume(solidLeadBox, Lead, "logicLead");
    G4VPhysicalVolume *physLead  = new G4PVPlacement(0, G4ThreeVector(0., 0., 3.*m), logicLead, "physLead", logicworld, false, 0., true);

    // CREATE THE FIBER
    G4double FiberBoxSizeXY = 0.5 * m;
    G4double FiberBoxSizeZ = 0.5 * m;
    G4Box* solidFiberBox = new G4Box("solidFiberBox", FiberBoxSizeXY/2, FiberBoxSizeXY/2, FiberBoxSizeZ/2);
	G4LogicalVolume *logicFiber = new G4LogicalVolume(solidFiberBox, polystyrene_fibre, "logicFiber");
    G4VPhysicalVolume *physFiber  = new G4PVPlacement(0, G4ThreeVector(0., 1.25*m, 1.*m), logicFiber, "physFiber", logicworld, false, 0., true);

    // DETECTOR
    G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	G4Material *H20 = new G4Material("H20", 1.000*g/cm3, 2);
	H20->AddElement(nist->FindOrBuildElement("H"), 2);
	H20->AddElement(nist->FindOrBuildElement("O"), 1);

	//G4Element *C = nist->FindOrBuildElement("C");

	G4Material *Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5*perCent);
	Aerogel->AddMaterial(H20, 37.4*perCent);
	Aerogel->AddElement(C, 0.1*perCent);

	G4double energies[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
	G4double rindexAerogel[2] = {1.1, 1.1};

	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energies, rindexAerogel, 2);

	Aerogel->SetMaterialPropertiesTable(mptAerogel); 

	// Create the detector
	G4double DetectorBoxSizeXY = 0.5 * m;
    G4double DetectorBoxSizeZ = 0.5 * m;

    G4Box* solidDetector = new G4Box("solidDetector", DetectorBoxSizeXY/2, DetectorBoxSizeXY/2, DetectorBoxSizeZ/2);
	logicDetector = new G4LogicalVolume(solidDetector, Aerogel, "logicDetector");
    G4VPhysicalVolume *physDetector  = new G4PVPlacement(0, G4ThreeVector(0., 1.75*m, 1.*m), logicDetector, "physDetector", logicworld, false, 0., true);

    return physworld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

	logicDetector->SetSensitiveDetector(sensDet);
}
