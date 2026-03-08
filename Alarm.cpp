//
// Created by rushd on 3/8/26.
//

#include "Alarm.h"
#include <iostream>

Alarm::Alarm() : isActive(false), volume(5), soundType("default"), lastActivatedAt("") {}

Alarm::Alarm(int volume, const std::string& soundType)
    : isActive(false), volume(volume), soundType(soundType), lastActivatedAt("") {}

void Alarm::activate() {
    isActive = true;
    std::cout << "Alarm activated" << std::endl;
}

void Alarm::activate(int durationSec) {
    isActive = true;
    std::cout << "Alarm activated for " << durationSec << " seconds" << std::endl;
}

void Alarm::deactivate() {
    isActive = false;
    std::cout << "Alarm deactivated" << std::endl;
}

void Alarm::testAlarm() {
    std::cout << "Testing alarm sound: " << soundType << std::endl;
}

bool Alarm::getStatus() const {
    return isActive;
}