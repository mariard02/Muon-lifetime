#include "EventAction.hh"

EventAction::EventAction(SteppingAction* steppingAction)
: G4UserEventAction(), fSteppingAction(steppingAction) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    G4int eventID = event->GetEventID();
    fSteppingAction->SetEventID(eventID); 
}

void EventAction::EndOfEventAction(const G4Event* /* event */)
{

}

