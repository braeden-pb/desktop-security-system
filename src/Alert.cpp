//
// Created by Michael Pachowski on 2026-03-06.
//

#include "Alert.h"
#include "SecuritySystem.h"
#include <iostream>

Alert::Alert(Event event, SecuritySystem& system)
    : event(event),system(system) {}

void Alert::sendAlert() {
    system.triggerAlert(event.getType());
}

Event Alert::getEvent() {
    return event;
}

void Alert::discard() {
    std::cout << "Alert discarded" << std::endl;
}

void Alert::update(const std::string& eventMsg) {
    if (eventMsg == "Alarm triggered") {
        sendAlert();
    } else if (eventMsg == "Motion detected") {
        sendAlert();
    }
}

void Alert::motionDetected() {
    system.soundAlert();
}
