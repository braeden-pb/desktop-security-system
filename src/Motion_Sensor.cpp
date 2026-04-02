/**
* @file Motion_Sensor.cpp
 * @brief Implementation of the Motion_Sensor class for software-side motion
 *        detection state management and camera trigger logic on the PC.
 * @author Braeden Patierno-Barker
 * @date 2026-03-22
 */

#include "Motion_Sensor.h"
#include <iostream>
#include "Camera.h"
/**
 * @brief Constructs a Motion_Sensor with default sensitivity and inactive state.
 *
 * Initializes all detection state flags to inactive and stores references to
 * the Config and Camera instances used for capture triggering.
 *
 * @param config Reference to the Config instance used to read capture mode,
 *               photos-per-event, clip duration, and sensitivity settings.
 * @param camera Reference to the Camera instance used to trigger photo capture
 *               and video recording on motion events.
 */
Motion_Sensor::Motion_Sensor(Config &config, Camera &camera) : sensitivity(5), motionDetected(false), lastDetected(0),
                                                       lastState(false), rearmPending(false), motionCount(0), rearmDelayMs(2000), config(config),camera(camera){
}
/**
 * @brief Destructs the Motion_Sensor object.
 *
 * No explicit cleanup required beyond what the member destructors handle.
 */
Motion_Sensor::~Motion_Sensor() {}
/**
 * @brief Returns the name identifier of this sensor.
 *
 * @return The string "Motion_Sensor".
 */
std::string Motion_Sensor::getName() const {
    return "Motion_Sensor";
}
/**
 * @brief Registers an observer to receive motion event notifications.
 *
 * Added observers are notified via their update() method whenever
 * notifyObservers() is called.
 *
 * @param o Pointer to the Observer instance to register.
 */
void Motion_Sensor::addObserver(Observer* o) {
    observers.push_back(o);
}
/**
 * @brief Notifies all registered observers of a motion event.
 *
 * Iterates through the observers list and calls update() on each one
 * with the provided event string.
 *
 * @param event The event string to broadcast to all observers.
 */
void Motion_Sensor::notifyObservers(const std::string& event) {
    for (auto* o : observers)
        o->update(event);
}
/**
 * @brief Activates the motion sensor and begins detection.
 *
 * Marks the sensor as connected and calls detectMotion() to begin
 * processing motion state.
 *
 * @note detectMotion() is currently called directly; a future improvement
 *       may move this call into a dedicated thread.
 */
void Motion_Sensor::activate() {

    setConnected(true);
    detectMotion(); // maybe call in thread?
}
/**
 * @brief Returns the current connection status of the sensor.
 *
 * @return "Connected" if the sensor is active, "Disconnected" otherwise.
 */
std::string Motion_Sensor::getStatus() const {
    if (isConnected()) {
        return "Connected";
    }
    return "Disconnected";
}
/**
 * @brief Marks the sensor as connected.
 *
 * Sets the connected flag to true. The ip and port parameters are
 * accepted for interface compatibility but not used in this implementation.
 *
 * @param ip   The IP address string (unused).
 * @param port The port number (unused).
 * @return true always.
 */
bool Motion_Sensor::connect(const std::string& ip, int port) {
    setConnected(true);
    return true;
}
/**
 * @brief Deactivates the sensor and resets all detection state.
 *
 * Marks the sensor as disconnected and clears motionDetected,
 * rearmPending, and lastState flags.
 */
void Motion_Sensor::disconnect() {
    //disconnect code here
    setConnected(false);
    motionDetected = false;
    rearmPending = false;
    lastState = false;
}
/**
 * @brief Updates the motion detection state and triggers camera actions if configured.
 *
 * Called whenever a new motion state reading arrives. On a detected motion:
 * -# Increments motionCount and sets the motionDetected flag.
 * -# Notifies all registered observers with "Motion detected".
 * -# Records the current time in lastDetected.
 * -# If capture mode is enabled and motionCount is a multiple of
 *    Config::getPhotosPer(), captures a photo and starts a video recording.
 *    A detached thread stops the recording after Config::getSeconds() seconds.
 *
 * On a falling edge (currentState false, lastState true), clears motionDetected.
 * Always updates lastState to currentState at the end.
 *
 * @param currentState true if motion is currently detected, false otherwise.
 *
 * @note The auto-stop recording thread is detached. If disconnect() or the
 *       destructor is called before it fires, Camera::stopRecording() will
 *       still be called but the early-return guard in stopRecording() prevents
 *       any double-stop.
 */
void Motion_Sensor::updateState(bool currentState) {
    if (currentState) {
        ++motionCount;
        motionDetected = true;
        notifyObservers("Motion detected");
        lastDetected = std::time(nullptr);
        if (motionCount % config.getPhotoFreq() == 0 &&config.getCaptureMode()) {
            for (int i=0; i<config.getPhotosPer()-1; i++) {
                camera.capturePhoto();

            }


        }
    }

    if (!currentState && lastState) {
        motionDetected = false;
    }

    lastState = currentState;
}
/**
 * @brief Returns the current motion detection state.
 *
 * @return true if motion is currently detected, false otherwise.
 */
bool Motion_Sensor::detectMotion() {
    return motionDetected;
}
/**
 * @brief Handles a direct motion trigger, logging and updating state.
 *
 * Prints the current motion count to standard output and calls
 * updateState(true) to process the motion event.
 *
 * @note This method is intended to be called by the network layer or
 *       observer system when the Pi reports a motion event.
 */
void Motion_Sensor::onMotion() {
    std::cout << motionCount << std::endl;
    updateState(true);
}
/**
 * @brief Polls and refreshes motion and rearm state.
 *
 * Updates lastDetected if motion is currently active, and clears the
 * rearmPending flag once the rearm timer has elapsed.
 *
 * @note Intended to be called periodically from a timer or update loop.
 */
void Motion_Sensor::updateStatus() {
    if (motionDetected) {
        lastDetected = std::time(nullptr);
    }
    if (rearmPending && std::chrono::steady_clock::now() >= rearmUntil) {
        rearmPending = false;
    }
}

/**
 * @brief Returns the current motion sensitivity level.
 *
 * @return Integer sensitivity value.
 */
int Motion_Sensor::getSensitivity() {
    return sensitivity;
}
/**
 * @brief Sets the motion sensitivity level.
 *
 * @param newSensitivity The new sensitivity value to apply.
 */
void Motion_Sensor::setSensitivity(int newSensitivity) {
    sensitivity = newSensitivity;
}