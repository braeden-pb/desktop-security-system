#include "Alarm.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <filesystem>

Alarm::Alarm()
    : isActive(false), volume(10), soundType("wav"), lastActivatedAt("") {}

/**
 * @brief Constructs an Alarm object with default settings.
 *
 * Initializes the alarm as inactive, with a volume of 10,
 * sound type set to "wav", and an empty last-activated timestamp.
 */
bool Alarm::loadSound(sf::SoundBuffer& buffer) {
    std::string soundPath = "../sounds/alarm.wav";
    std::cout << "Loading Sound From: " << soundPath << std::endl;
    return buffer.loadFromFile(soundPath);
}

/**
 * @brief Loads the alarm sound from disk into the provided buffer.
 *
 * Attempts to load the alarm audio file from a relative path.
 * Prints the resolved path to standard output for debugging purposes.
 *
 * @param[out] buffer The SFML SoundBuffer to load the audio data into.
 * @return true  if the sound file was loaded successfully.
 * @return false if the file could not be found or loaded.
 */
void Alarm::activate() {
    isActive = true;
    std::cout << "Alarm activated!" << std::endl;


    if (!loadSound(buffer)) {
        std::cout << "Failed to load sound." << std::endl;
        return;
    }
    sound.emplace(buffer);
    sound->setLooping(true);
    sound->play();
}


/**
 * @brief Activates the alarm and begins playing the alarm sound.
 *
 * Sets the alarm state to active, loads the sound file into the
 * internal buffer, and starts looped playback. If the sound file
 * fails to load, the function logs an error and returns early
 * without playing audio.
 *
 * @note The sound will loop continuously until @ref deactivate() is called.
 */void Alarm::deactivate() {
    isActive = false;
    sound->stop();
    std::cout << "Alarm deactivated." << std::endl;
}

/**
 * @brief Deactivates the alarm and stops audio playback.
 *
 * Sets the alarm state to inactive and immediately stops
 * any currently playing alarm sound.
 *
 * @pre The alarm must have been previously activated via @ref activate().
 */

void Alarm::testAlarm() {
    std::cout << "Testing alarm..." << std::endl;
    activate();
}


/**
 * @brief Triggers a test of the alarm sound.
 *
 * Calls @ref activate() to verify that audio playback
 * is functioning correctly. Intended for diagnostic use.
 *
 * @note This does not automatically stop the alarm after testing.
 *       Call @ref deactivate() manually to stop playback.
 */
bool Alarm::getStatus() const {
    return isActive;
}
void Alarm::update(const std::string& event) {
    if (event == "Alarm triggered") {
        activate();
    }
}
