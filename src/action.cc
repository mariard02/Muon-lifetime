#include "action.hh"

MyactionInitialization::MyactionInitialization()
{}

MyactionInitialization::~MyactionInitialization()
{}

void MyactionInitialization::Build() const
{
	MyPrimaryGenerator *generator = new MyPrimaryGenerator();
	SetUserAction(generator);
}