#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	RegisterPhysics (new G4EmStandardPhysics(0));
	RegisterPhysics (new G4OpticalPhysics(0));
	RegisterPhysics (new G4DecayPhysics(0));
	RegisterPhysics (new G4RadioactiveDecayPhysics(0));
	RegisterPhysics (new G4HadronPhysicsFTFP_BERT(0));
	RegisterPhysics (new G4HadronPhysicsQGSP_BERT(0));
};

MyPhysicsList::~MyPhysicsList(){}; 
