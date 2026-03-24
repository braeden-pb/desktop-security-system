//
// Created by evan on 2026-03-24.
//

#include "Motion_Sensor_PI.h"


const int PIR_PIN = 17;
Motion_Sensor_PI* Motion_Sensor_PI::instance_ = nullptr;

Motion_Sensor_PI::Motion_Sensor_PI(int sensitivity, int motionSleep) : active(false),motionDetected(false), sensitivity(sensitivity), motionSleep(motionSleep){
    instance_ = this;
    if (wiringPiSetupGpio() == -1) {
        throw std::runtime_error("Failed to initialize WiringPi");
    }

    pinMode(PIR_PIN, INPUT);
}

void Motion_Sensor_PI::activate() {
    {

        std::lock_guard<std::mutex> lock(mtx);
        active = true;
        wiringPiISR(PIR_PIN, INT_EDGE_BOTH, &Motion_Sensor_PI::isrHandler);
	detectMotion();
    }
    //sensorThread = std::thread(&Motion_Sensor_PI::detectMotion, this);
}

void Motion_Sensor_PI::detectMotion() {
    std::cout << "detectMotion started\n";
    while (active) {
        int pinState = digitalRead(PIR_PIN);
        std::cout << "PIN STATE: " << pinState << "\n";  // see what pin is reading

        bool currentState = pinState == HIGH;
        if (currentState && !motionDetected) {
            motionDetected = true;
            onMotion();
        } else if (!currentState) {
            motionDetected = false;
        }
        usleep(500000);  // slow it down so you can read the output
    }
}

void Motion_Sensor_PI::onMotion() {
    motionDetected = true;
    std::cout << "Motion detected!\n";
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
    std::lock_guard<std::mutex> lock(mtx);
    active = false;

    if (sensorThread.joinable()) {
        sensorThread.join();
    }
    //motionDetected;
}


Motion_Sensor_PI::~Motion_Sensor_PI(){}
