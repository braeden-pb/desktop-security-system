/**
 * @file Alert.cpp
 * @brief Implementation of the Alert class for forwarding security events
 *        to the SecuritySystem.
 * @author Braeden Patierno-Barker
 * @date 2026-03-31
 */

#include "Alert.h"
#include "SecuritySystem.h"
#include <iostream>

/**
 * @brief Constructs an Alert object bound to a specific event and security system.
 *
 * @param event  The Event object this alert is associated with.
 * @param system Reference to the SecuritySystem instance that handles
 *               alert triggering and sound notifications.
 */
Alert::Alert(Event event, SecuritySystem& system)
    : event(event),system(system) {}
/**
 * @brief Sends an alert to the SecuritySystem based on the associated event type.
 *
 * Forwards the event type retrieved from the internal event object to
 * SecuritySystem::triggerAlert().
 */
void Alert::sendAlert() {
    system.triggerAlert(event.getType());
}
/**
 * @brief Discards the current alert without forwarding it to the SecuritySystem.
 *
 * Logs a message to standard output indicating the alert was discarded.
 * No further action is taken on the event.
 */
Event Alert::getEvent() {
    return event;
}

/**
 * @brief Discards the current alert without forwarding it to the SecuritySystem.
 *
 * Logs a message to standard output indicating the alert was discarded.
 * No further action is taken on the event.
 */
void Alert::discard() {
    std::cout << "Alert discarded" << std::endl;
}
/**
 * @brief Observer update method that reacts to incoming security event messages.
 *
 * Called by the observed subject when a monitored event occurs. Triggers
 * sendAlert() for both "Alarm triggered" and "Motion detected" events.
 * Unrecognized event strings are silently ignored.
 *
 * @param eventMsg The event string dispatched by the observed subject.
 */
void Alert::update(const std::string& eventMsg) {
    if (eventMsg == "Alarm triggered") {
        sendAlert();
    } else if (eventMsg == "Motion detected") {
        sendAlert();
    }
}

/**
 * @brief Handles a motion detection event by triggering an audible alert.
 *
 * Calls SecuritySystem::soundAlert() directly, bypassing the observer
 * event string mechanism used by update().
 *
 * @note This method can be called independently of the observer pattern
 *       when a motion event needs to be handled directly.
 */
void Alert::motionDetected() {
    system.soundAlert();
}
