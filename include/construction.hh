#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4SystemOfUnits.hh"

#include "detector.hh"

#include "G4HadronPhysicsFTFP_BERT.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume *Construct();

private:
	G4LogicalVolume *logicDetector;
	virtual void ConstructSDandField();

};

#endif