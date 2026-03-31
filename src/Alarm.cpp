#include "Alarm.h"
#include <iostream>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>
#include <filesystem>

#include "Config.h"

/**
 * @brief Constructs an Alarm object with default settings.
 *
 * Initializes the alarm as inactive, with a volume of 10,
 * sound type set to "wav", and an empty last-activated timestamp.
 */
Alarm::Alarm(Network &network,Config &config)
    : isActive(false), soundType("wav"), lastActivatedAt(""),network(network),config(config) {}

Alarm::~Alarm() {}



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
    if (isActive) return;
    isActive = true;
    std::cout << "Alarm activated!" << std::endl;

    std::string soundOption = config.getSound();


    PacketHeader header{};
    header.system      = System::Speaker;
    header.command     = Command::soundAlarm;
    header.payloadSize = static_cast<uint32_t>(soundOption.size());
    std::vector<uint8_t> payload(soundOption.begin(), soundOption.end());
    network.send(header, payload);

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    lastActivatedAt = oss.str();

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
    if (!isActive) return;
    isActive = false;
    PacketHeader header{};
    header.system      = System::Speaker;
    header.command     = Command::soundAlarm;
    header.payloadSize = 0;

    network.send(header, {});
    std::cout << "Alarm deactivated." << std::endl;
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
    if (event == "Motion detected" && config.getAlarmOnMotion()) {
        activate();
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::seconds(config.getMaxAlarmDuration()));
            deactivate();
        }).detach();
    }
}
