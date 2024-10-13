#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName = "mu-";
	G4ParticleDefinition *particle = particleTable->FindParticle("mu-");

	G4ThreeVector pos(0., 0., -4.*m);
	G4ThreeVector mom(0., 0., 1.);

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(4000. * MeV);
	fParticleGun->SetParticleDefinition(particle);

	fParticleGun->GeneratePrimaryVertex(anEvent);
}