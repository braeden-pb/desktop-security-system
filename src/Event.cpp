/**
* @file Event.cpp
 * @brief Implementation of the Event class representing a security system event
 *        with an associated timestamp and optional type.
 * @author evan, Rushd
 * @date 2026-03-21
 */
#include "Event.h"
/**
 * @brief Constructs an Event with a timestamp only.
 *
 * Creates an event without a type, suitable for generic or
 * unclassified system events.
 *
 * @param time A string representing the time the event occurred.
 */
Event::Event(std::string time) {
    timeStamp = time;
}

/**
 * @brief Constructs an Event with a timestamp and a type.
 *
 * Creates a fully described event with both a time of occurrence
 * and a classification string (e.g. "Motion detected", "Alarm triggered").
 *
 * @param time A string representing the time the event occurred.
 * @param type A string identifying the category or nature of the event.
 */
Event::Event(std::string time, std::string type) {
    timeStamp = time;
    this->type = type;
}

/**
 * @brief Returns the timestamp of the event.
 *
 * @return A string containing the time at which the event occurred.
 */
std::string Event::getTime() const{
    return timeStamp;
}

/**
 * @brief Returns the type of the event.
 *
 * @return A string identifying the category or nature of the event.
 *         Returns an empty string if no type was set at construction.
 */
std::string Event::getType() const {
    return type;
}
