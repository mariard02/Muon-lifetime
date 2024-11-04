#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	RegisterPhysics (new G4EmStandardPhysics());
	RegisterPhysics (new G4OpticalPhysics());
	RegisterPhysics (new G4DecayPhysics());
	RegisterPhysics (new G4RadioactiveDecayPhysics());
	RegisterPhysics (new G4HadronPhysicsFTFP_BERT());
	RegisterPhysics (new G4HadronPhysicsQGSP_BERT());
};

MyPhysicsList::~MyPhysicsList(){}; 
