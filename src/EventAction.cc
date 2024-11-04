#include "EventAction.hh"

EventAction::EventAction(SteppingAction* steppingAction)
: G4UserEventAction(), fSteppingAction(steppingAction) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    G4int eventID = event->GetEventID();
    fSteppingAction->SetEventID(eventID); // Pasa el eventID a SteppingAction

    G4cout << "NEW EVENT \n";
}

void EventAction::EndOfEventAction(const G4Event* event) {
    // Aquí puedes realizar acciones al finalizar cada evento si es necesario
}