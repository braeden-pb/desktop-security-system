//
// Created by Michael Pachowski on 2026-03-06.
//

#include "Alert.h"
#include "SecuritySystem.h"
#include <iostream>

Alert::Alert(Event& event, std::string email, SecuritySystem& system)
    : event(event), email(email), system(system) {}

void Alert::sendAlert() {
    std::cout << "Alert sent to " << email << " at " << event.getTime() << std::endl;
}

Event Alert::getEvent() {
    return event;
}

void Alert::discard() {
    std::cout << "Alert discarded" << std::endl;
}