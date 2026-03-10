//
// Created by rushd on 3/8/26.
// updated by rushd on 3/10/2026
//

#include "Alarm.h"
#include <iostream>
#include <chrono>

// Construct the alarm with default settings
Alarm::Alarm()
    : isActive(false), volume(10), soundType("mp3"), lastActivatedAt("") {}

// Activates the alarm until manually stopped
void Alarm::activate() {
    isActive = true;
    std::cout << "Alarm activated!" << std::endl;

    // Play the alarm sound file
    system("mpg321 ../sounds/alarm.mp3");
}

// Activates the alarm for a specific duration in seconds
void Alarm::activate(int durationSec) {
    isActive = true;
    std::cout << "Alarm activated for " << durationSec << " seconds" << std::endl;

    // Play the alarm sound once per second for the given duration
    for (int i = 0; i < durationSec; i++) {
        system("mpg321 ../sounds/alarm.mp3");
    }
}

// Stops the alarm and updates its status
void Alarm::deactivate() {
    isActive = false;
    std::cout << "Alarm deactivated." << std::endl;
}

// Used to verify that the alarm sound works correctly
void Alarm::testAlarm() {
    std::cout << "Testing alarm..." << std::endl;
    activate(3);
}

// Returns the current status of the alarm
bool Alarm::getStatus() const {
    return isActive;
}