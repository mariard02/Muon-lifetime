#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{	
	G4double separation = 0.75; // Separation between the lead and the first scintillator IN METERS
	G4NistManager *nist = G4NistManager::Instance();

	// World material: air
	G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");

	G4MaterialPropertiesTable* MPT_air = new G4MaterialPropertiesTable();
	std::vector<G4double> energyAir     = {2.034*eV, 3.*eV, 4.136*eV};
	std::vector<G4double> rindexAir     = {1., 1., 1.};
	MPT_air ->AddProperty("RINDEX", energyAir, rindexAir);
	//MPT_air->AddConstProperty("RINDEX", 1., true);
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
	MPT->AddConstProperty("SCINTILLATIONYIELD", 10000/MeV); // 10000
	
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
	//G4Material* Lead = nist->FindOrBuildMaterial("G4_Pb");
	//G4Material* Lead= new G4Material("Lead", 82., 207.2 *g/mole, 11.35);
	G4Material* Lead = new G4Material("Lead", 82, 207.2 * g/mole, 11.35 * g/cm3);
	G4Material* Iron = new G4Material("Iron", 26, 55.847 * g/mole, 7.874 * g/cm3);
	G4Material* Al = new G4Material("Al", 13, 26.98 * g/mole, 2.698 * g/cm3);


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
	G4Box *solidworld = new G4Box("solidworld", 2.*m, 2.*m, 2.*m);
	G4LogicalVolume *logicworld = new G4LogicalVolume(solidworld, worldMaterial, "logicWorld");
	G4VPhysicalVolume *physworld  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicworld, "physworld", 0, false, 0., true);

	// CREATE THE SCINTILLATOR
	G4double ScinBoxSizeY = 30. * cm;
	G4double ScinBoxSizeX = 15. * cm;
    G4double ScinBoxSizeZ  = 1. * cm;
    G4Box* solidScinBox = new G4Box("solidScinBox", 0.5*ScinBoxSizeX, 0.5*ScinBoxSizeY, 0.5*ScinBoxSizeZ);
	
    G4LogicalVolume *logicScin = new G4LogicalVolume(solidScinBox, scintillator, "logicScin");

    G4VPhysicalVolume *physScin0 = new G4PVPlacement(0, G4ThreeVector(0., 0., - 5. * cm), logicScin, "physScin", logicworld, false, 0., true);
	G4VPhysicalVolume *physScin1 = new G4PVPlacement(0, G4ThreeVector(0., 0., - 4. * cm), logicScin, "physScin", logicworld, false, 1., true);
	G4VPhysicalVolume *physScin2 = new G4PVPlacement(0, G4ThreeVector(0., 0., 4. * cm), logicScin, "physScin", logicworld, false, 2., true);
	G4VPhysicalVolume *physScin3 = new G4PVPlacement(0, G4ThreeVector(0., 0., 5. * cm), logicScin, "physScin", logicworld, false, 3., true);

    // Optical surface: mirror between the scintillator and the air.
    G4OpticalSurface* opAirScintillator = new G4OpticalSurface("AirScintillator");
    opAirScintillator -> SetType(dielectric_metal);
    opAirScintillator -> SetFinish(ground);
    opAirScintillator -> SetModel(unified);
    opAirScintillator -> SetPolish(0.8);

    G4MaterialPropertiesTable* OpSurfaceProperty = new G4MaterialPropertiesTable();

    std::vector<G4double> pp = {2.038*eV, 4.144*eV};
    std::vector<G4double> reflectivity = {1., 1.};
    std::vector<G4double> transmittance = {0., 0.};
    //std::vector<G4double> efficiency = {0.1, 0.1};

    OpSurfaceProperty -> AddProperty("REFLECTIVITY", pp, reflectivity);
    OpSurfaceProperty->AddProperty("TRANSMITTANCE", pp, transmittance);
    //OpSurfaceProperty -> AddProperty("EFFICIENCY", pp, efficiency);

    opAirScintillator -> SetMaterialPropertiesTable(OpSurfaceProperty);

    //G4LogicalSkinSurface* airScintillator =
		//new G4LogicalSkinSurface("AirScintillator", logicScin, opAirScintillator);

    G4LogicalBorderSurface* AirScintillator0 = new G4LogicalBorderSurface("AirScintillator", physScin0, physworld, opAirScintillator);
	G4LogicalBorderSurface* AirScintillator1 = new G4LogicalBorderSurface("AirScintillator", physScin1, physworld, opAirScintillator);
	G4LogicalBorderSurface* AirScintillator2 = new G4LogicalBorderSurface("AirScintillator", physScin2, physworld, opAirScintillator);
	G4LogicalBorderSurface* AirScintillator3 = new G4LogicalBorderSurface("AirScintillator", physScin3, physworld, opAirScintillator);


    // CREATE THE LEAD
    G4double LeadBoxSizeY = 40. * cm;
	G4double LeadBoxSizeX = 25. * cm;
	G4double LeadBoxSizeZ = 2 * cm; // Depth of the box in Z direction
	G4Box* solidLeadBox = new G4Box("solidLeadBox", LeadBoxSizeX/2, LeadBoxSizeY/2, LeadBoxSizeZ/2);
	G4LogicalVolume *logicLead = new G4LogicalVolume(solidLeadBox, Iron, "logicLead");
    G4VPhysicalVolume *physLead  = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.*m), logicLead, "physLead", logicworld, false, 0., true);

    // CREATE THE FIBER
    G4double FiberBoxSizeX = 14. * cm;
	G4double FiberBoxSizeYmin = 1. * cm;
	G4double FiberBoxSizeYmax = 1. * cm;
    G4double FiberBoxSizeZmin = 5. * cm;
	G4double FiberBoxSizeZmax = 15. * cm;
    //G4Box* solidFiberBox = new G4Box("solidFiberBox", FiberBoxSizeXY/2, FiberBoxSizeXY/2, FiberBoxSizeZ/2);

	G4Trd* solidFiberBox = new G4Trd("solidFiberBox", 0.5 * FiberBoxSizeYmin,  // Semi-ancho inferior
                                0.5 * FiberBoxSizeYmax,  // Semi-ancho superior
                                0.5 * FiberBoxSizeZmin,  // Semi-alto inferior
                                0.5 * FiberBoxSizeZmax,  // Semi-alto superior
                                0.5 * FiberBoxSizeX);

	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateX(90. * deg);
	rotation->rotateZ(90. * deg);

	G4RotationMatrix* rotation180 = new G4RotationMatrix(*rotation);
	rotation180->rotateX(180. * deg);

	G4LogicalVolume *logicFiber = new G4LogicalVolume(solidFiberBox, polystyrene_fibre, "logicFiber");
    G4VPhysicalVolume *physFiber0  = new G4PVPlacement(rotation180, G4ThreeVector(0., 22. * cm, - 5. * cm), logicFiber, "physFiber", logicworld, false, 0., true);
	G4VPhysicalVolume *physFiber1  = new G4PVPlacement(rotation, G4ThreeVector(0., - 22. * cm, - 4. * cm), logicFiber, "physFiber", logicworld, false, 1., true);
	G4VPhysicalVolume *physFiber2  = new G4PVPlacement(rotation180, G4ThreeVector(0., 22. * cm, 4. * cm), logicFiber, "physFiber", logicworld, false, 2., true);
	G4VPhysicalVolume *physFiber3  = new G4PVPlacement(rotation, G4ThreeVector(0., - 22. * cm, 5. * cm), logicFiber, "physFiber", logicworld, false, 3., true);

    // Optical surface: mirror between the fiber and the air

    G4OpticalSurface* opAirFiber = new G4OpticalSurface("AirFiber");
    opAirFiber -> SetType(dielectric_metal);
    opAirFiber -> SetFinish(polished);
    opAirFiber -> SetModel(glisur);
    opAirFiber -> SetPolish(0.8);

    G4MaterialPropertiesTable* OpSurfacePropertyFiber = new G4MaterialPropertiesTable();

    OpSurfacePropertyFiber -> AddProperty("REFLECTIVITY", pp, reflectivity);
    OpSurfacePropertyFiber -> AddProperty("TRANSMITTANCE", pp, transmittance);
    //OpSurfaceProperty -> AddProperty("EFFICIENCY", pp, efficiency);

    opAirFiber -> SetMaterialPropertiesTable(OpSurfacePropertyFiber);

    G4LogicalBorderSurface* AirFiber0 = new G4LogicalBorderSurface("AirFiber", physFiber0, physworld, opAirFiber);
	G4LogicalBorderSurface* AirFiber1 = new G4LogicalBorderSurface("AirFiber", physFiber1, physworld, opAirFiber);
	G4LogicalBorderSurface* AirFiber2 = new G4LogicalBorderSurface("AirFiber", physFiber2, physworld, opAirFiber);
	G4LogicalBorderSurface* AirFiber3 = new G4LogicalBorderSurface("AirFiber", physFiber3, physworld, opAirFiber);

    // DETECTOR
    G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	G4Material *H20 = new G4Material("H20", 1.000*g/cm3, 2);
	H20->AddElement(nist->FindOrBuildElement("H"), 2);
	H20->AddElement(nist->FindOrBuildElement("O"), 1);

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
	G4double DetectorBoxSizeX = 5 * cm;
	G4double DetectorBoxSizeY = 10. * cm;
    G4double DetectorBoxSizeZ = 1. * cm;

    G4Box* solidDetector = new G4Box("solidDetector", DetectorBoxSizeX/2, DetectorBoxSizeY/2, DetectorBoxSizeZ/2);
	logicDetector = new G4LogicalVolume(solidDetector, Aerogel, "logicDetector");
    G4VPhysicalVolume *physDetector0  = new G4PVPlacement(0, G4ThreeVector(0., 34. * cm, - 5. * cm), logicDetector, "physDetector", logicworld, false, 0, true);
	G4VPhysicalVolume *physDetector1  = new G4PVPlacement(0, G4ThreeVector(0., -34. * cm, - 4. * cm), logicDetector, "physDetector", logicworld, false, 1, true);
	G4VPhysicalVolume *physDetector2  = new G4PVPlacement(0, G4ThreeVector(0., 34. * cm, 4. * cm), logicDetector, "physDetector", logicworld, false, 2, true);
	G4VPhysicalVolume *physDetector3  = new G4PVPlacement(0, G4ThreeVector(0., -34. * cm, 5. * cm), logicDetector, "physDetector", logicworld, false, 3, true);
    
    G4OpticalSurface* opAirDetector = new G4OpticalSurface("AirDetector");
    opAirDetector -> SetType(dielectric_metal);
    opAirDetector -> SetFinish(polished);
    opAirDetector -> SetModel(glisur);
    opAirDetector -> SetPolish(0.8);

    G4MaterialPropertiesTable* OpSurfacePropertyDetector = new G4MaterialPropertiesTable();

    OpSurfacePropertyDetector -> AddProperty("REFLECTIVITY", pp, reflectivity);
    OpSurfacePropertyDetector -> AddProperty("TRANSMITTANCE", pp, transmittance);
    //OpSurfaceProperty -> AddProperty("EFFICIENCY", pp, efficiency);

    opAirDetector -> SetMaterialPropertiesTable(OpSurfacePropertyDetector);

    G4LogicalBorderSurface* AirDetector0 = new G4LogicalBorderSurface("AirDetector", physDetector0, physworld, opAirDetector);
	G4LogicalBorderSurface* AirDetector1 = new G4LogicalBorderSurface("AirDetector", physDetector1, physworld, opAirDetector);
	G4LogicalBorderSurface* AirDetector2 = new G4LogicalBorderSurface("AirDetector", physDetector2, physworld, opAirDetector);
	G4LogicalBorderSurface* AirDetector3 = new G4LogicalBorderSurface("AirDetector", physDetector3, physworld, opAirDetector);

    return physworld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	//MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

	//logicDetector->SetSensitiveDetector(sensDet);
}
