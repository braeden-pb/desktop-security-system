/**
 * @file Alarm.cpp
 * @brief Implementation of the Alarm class for managing alarm state and signalling
 *        the Raspberry Pi speaker over the network.
 * @author evan, Rushd
 * @date 2026-03-21
 */
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
 * Initializes the alarm as inactive, with sound type set to "wav",
 * an empty last-activated timestamp, and references to the network
 * and config dependencies.
 *
 * @param network Reference to the Network instance used to send alarm
 *                commands to the Raspberry Pi.
 * @param config  Reference to the Config instance used to read alarm
 *                sound, duration, and motion trigger settings.
 */
Alarm::Alarm(Network &network,Config &config)
    : isActive(false), soundType("wav"), lastActivatedAt(""),network(network),config(config) {}
/**
 * @brief Destructs the Alarm object.
 *
 * No explicit cleanup required; any active alarm should be stopped
 * by calling deactivate() before destruction.
 */
Alarm::~Alarm() {}




/**
 * @brief Activates the alarm and signals the Raspberry Pi to begin playing sound.
 *
 * Guards against double-activation with an early return if already active.
 * On activation:
 * -# Sets the active flag and notifies all registered observers with "Alarm triggered".
 * -# Reads the configured sound option from Config and sends a soundAlarm packet
 *    to the Pi with the sound name as the payload.
 * -# Records the current local time as a formatted timestamp in lastActivatedAt.
 *
 * @note To stop the alarm, call @ref deactivate().
 */
void Alarm::activate() {
    if (isActive) return;
    isActive = true;
    std::cout << "Alarm activated!" << std::endl;
    notifyObservers("Alarm triggered");

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
 * @brief Registers an observer to receive alarm event notifications.
 *
 * Added observers are notified via their update() method whenever
 * notifyObservers() is called.
 *
 * @param o Pointer to the Observer instance to register.
 */
void Alarm::addObserver(Observer* o) {
    observers.push_back(o);
}
/**
 * @brief Notifies all registered observers of an alarm event.
 *
 * Iterates through the observers list and calls update() on each one
 * with the provided event string.
 *
 * @param event The event string to broadcast to all observers.
 */
void Alarm::notifyObservers(const std::string &event) {
    for (auto* o : observers)
        o->update(event);
}


/**
 * @brief Deactivates the alarm and signals the Raspberry Pi to stop playing sound.
 *
 * Guards against redundant deactivation with an early return if already inactive.
 * Sends a soundAlarm packet with an empty payload to the Pi, instructing it to
 * silence the alarm on the hardware side.
 *
 * @pre The alarm should have been previously activated via @ref activate().
 */
void Alarm::deactivate() {
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
 * @brief Returns the current active state of the alarm.
 *
 * @return true  if the alarm is currently active.
 * @return false if the alarm is inactive.
 */
bool Alarm::getStatus() const {
    return isActive;
}
/**
 * @brief Observer update method that reacts to incoming security event strings.
 *
 * Handles two event types:
 * - **"Alarm triggered"**: Calls activate() unconditionally.
 * - **"Motion detected"**: Calls activate() only if Config::getAlarmOnMotion()
 *   is true, then spawns a detached thread that automatically calls deactivate()
 *   after the duration returned by Config::getMaxAlarmDuration().
 *
 * @param event The event string dispatched by the observed subject.
 *
 * @note The auto-deactivation thread is detached and runs independently.
 *       If deactivate() is called manually before the timer expires, the
 *       detached thread will still fire but the early-return guard in
 *       deactivate() will prevent any double-deactivation.
 */
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
