#include "Alarm.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>

// Construct the alarm with default settings
Alarm::Alarm()
    : isActive(false), volume(10), soundType("wav"), lastActivatedAt("") {}

// Activates the alarm
void Alarm::activate() {
    isActive = true;
    std::cout << "Alarm activated!" << std::endl;

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("../sounds/alarm.wav")) {
        std::cout << "Failed to load sound file." << std::endl;
        return;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Activates the alarm for a specific duration in seconds
void Alarm::activate(int durationSec) {
    isActive = true;
    std::cout << "Alarm activated for " << durationSec << " seconds" << std::endl;

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("../sounds/alarm.wav")) {
        std::cout << "Failed to load sound file." << std::endl;
        return;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    std::this_thread::sleep_for(std::chrono::seconds(durationSec));
    sound.stop();
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