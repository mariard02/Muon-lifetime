#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"

class MyactionInitialization : public G4VUserActionInitialization
{
public:
	MyactionInitialization();
	~MyactionInitialization();

	virtual void Build() const;
};

#endif