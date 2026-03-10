#include "Alarm.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <filesystem>

// Construct the alarm with default settings
Alarm::Alarm()
    : isActive(false), volume(10), soundType("wav"), lastActivatedAt("") {}


bool Alarm::loadSound(sf::SoundBuffer& buffer) {
    std::string soundPath = std::filesystem::current_path().string() + "/sounds/alarm.wav";
    std::cout << "Loading Sound From: " << soundPath << std::endl;
    return buffer.loadFromFile(soundPath);
}

// Activates the alarm
void Alarm::activate() {
    isActive = true;
    std::cout << "Alarm activated!" << std::endl;


    if (!loadSound(buffer)) {
        std::cout << "Failed to load sound." << std::endl;
        return;
    }
    sound.setBuffer(buffer);
    sound.setLoop(true);
    sound.play();
}


// Stops the alarm and updates its status
void Alarm::deactivate() {
    isActive = false;
    sound.stop();
    std::cout << "Alarm deactivated." << std::endl;
}

// Used to verify that the alarm sound works correctly
void Alarm::testAlarm() {
    std::cout << "Testing alarm..." << std::endl;
    activate();
}

// Returns the current status of the alarm
bool Alarm::getStatus() const {
    return isActive;
}