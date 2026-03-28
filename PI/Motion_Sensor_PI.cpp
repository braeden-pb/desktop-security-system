//
// Created by evan on 2026-03-24.
//

#include "Motion_Sensor_PI.h"

#include "../src/Network.h"


const int PIR_PIN = 17;
Motion_Sensor_PI* Motion_Sensor_PI::instance_ = nullptr;

Motion_Sensor_PI::Motion_Sensor_PI(int sensitivity, int motionSleep,Network &network) : active(false),motionDetected(false), sensitivity(sensitivity), motionSleep(motionSleep),network(network){
    instance_ = this;
    if (wiringPiSetupGpio() == -1) {
        throw std::runtime_error("Failed to initialize WiringPi");
    }

    pinMode(PIR_PIN, INPUT);
}

void Motion_Sensor_PI::activate() {
        active = true;
	    detectMotion();
    }

bool Motion_Sensor_PI::isActive() {
    return active;
}

void Motion_Sensor_PI::detectMotion() {
    std::cout << "detectMotion started\n";
        wiringPiISR(PIR_PIN, INT_EDGE_BOTH, &Motion_Sensor_PI::isrHandler);
}

void Motion_Sensor_PI::onMotion() {
    motionDetected = true;

    network.send()

}

bool Motion_Sensor_PI::isMotionDetected() {
    return motionDetected;
}

void Motion_Sensor_PI::isrHandler() {
    if (instance_) {
        bool detected = digitalRead(PIR_PIN) == HIGH;
        //std::lock_guard<std::mutex> lock(instance_->motionMutex);
        if (detected && !instance_->motionDetected) {
            instance_->motionDetected = true;
            instance_->onMotion();
        } else if (!detected) {
            instance_->motionDetected = false;
        }
    }
}

void Motion_Sensor_PI::deactivate() {
    active = false;

}


Motion_Sensor_PI::~Motion_Sensor_PI(){}
