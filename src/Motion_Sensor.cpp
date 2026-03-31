//
// Created by Braeden Patierno-Barker on 3/22/2026.
//

#include "Motion_Sensor.h"
#include <iostream>

Motion_Sensor::Motion_Sensor() : sensitivity(5), motionDetected(false), lastDetected(0),
      lastState(false), rearmPending(false), motionCount(0), rearmDelayMs(2000) {

}

Motion_Sensor::~Motion_Sensor() {}

std::string Motion_Sensor::getName() const {
    return "Motion_Sensor";
}

// void Motion_Sensor::updateState(bool currentState) {
//     if (motionDetected)
//         notifyObservers("Motion detected");
// }

void Motion_Sensor::addObserver(Observer* o) {
    observers.push_back(o);
}

void Motion_Sensor::notifyObservers(const std::string& event) {
    for (auto* o : observers)
        o->update(event);
}

void Motion_Sensor::activate() {

    setConnected(true);
    detectMotion(); // maybe call in thread?
}

std::string Motion_Sensor::getStatus() const {
    if (isConnected()) {
        return "Connected";
    }
    return "Disconnected";
}

bool Motion_Sensor::connect(const std::string& ip, int port) {
    setConnected(true);
    return true;
}

void Motion_Sensor::disconnect() {
    //disconnect code here
    setConnected(false);
    motionDetected = false;
    rearmPending = false;
    lastState = false;
}

void Motion_Sensor::updateState(bool currentState) {
    if (currentState) {
        ++motionCount;
        motionDetected = true;
        notifyObservers("Motion detected");
        lastDetected = std::time(nullptr);
    }

    if (!currentState && lastState) {
        motionDetected = false;
    }



    lastState = currentState;
}

bool Motion_Sensor::detectMotion() {
    //send signal to pi
    return motionDetected;
}

void Motion_Sensor::onMotion() {
    std::cout << motionCount << std::endl;
    updateState(true);
}

void Motion_Sensor::updateStatus() {
    if (motionDetected) {
        lastDetected = std::time(nullptr);
    }
    if (rearmPending && std::chrono::steady_clock::now() >= rearmUntil) {
        rearmPending = false;
    }
}

int Motion_Sensor::getSensitivity() {
    return sensitivity;
}

void Motion_Sensor::setSensitivity(int newSensitivity) {
    sensitivity = newSensitivity;
}