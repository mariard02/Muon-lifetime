#ifndef DETECTOR_HH
#define DETECTOR_HH 

#include "G4VSensitiveDetector.hh"
#include "RunAction.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String);
	~MySensitiveDetector();


private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
	std::ofstream OutputFile;
	std::ofstream newFile;

	G4int eventID;

	G4PhysicsFreeVector *quEff;

};

#endif