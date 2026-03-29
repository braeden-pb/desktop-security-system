//
// Created by evan on 2026-03-24.
//

#include "Motion_Sensor_PI.h"

#include "NetworkServer.h"

Motion_Sensor_PI* Motion_Sensor_PI::instance_ = nullptr;
const int PIR_PIN = 17;
Motion_Sensor_PI::Motion_Sensor_PI(int sensitivity, int motionSleep, NetworkServer& network)
    : active(false), motionDetected(false), sensitivity(sensitivity),
      motionSleep(motionSleep), network(network) {
    instance_ = this;
    if (wiringPiSetupGpio() == -1)
        throw std::runtime_error("Failed to initialize WiringPi");
    pinMode(PIR_PIN, INPUT);
    // removed thread from here
}

void Motion_Sensor_PI::activate() {
        active = true;
    sensorThread = std::thread([this]() {
        while (active) {
            if (motionPending) {
                motionPending = false;
                PacketHeader header{};
                header.system      = System::Motion;
                header.command     = Command::MotionDetected;
                header.payloadSize = 0;
                network.sendPacket(header, {});
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
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
    motionPending = true;  // just set flag, thread does the send
}

bool Motion_Sensor_PI::isMotionDetected() {
    return motionDetected;
}

void Motion_Sensor_PI::isrHandler() {
    if (!instance_) return;
    if (instance_->motionPending) return;  // ADD THIS - already waiting to send

    bool detected = digitalRead(PIR_PIN) == HIGH;

    if (detected) {
        time_t now = time(nullptr);
        if (difftime(now, instance_->lastSent) >= 5.0) {
            instance_->lastSent = now;
            instance_->motionDetected = true;
            instance_->motionPending = true;  // set flag directly, skip onMotion()
        }
    } else {
        instance_->motionDetected = false;
    }
}

void Motion_Sensor_PI::deactivate() {
    active = false;
    if (sensorThread.joinable())
        sensorThread.join();
}


Motion_Sensor_PI::~Motion_Sensor_PI(){}
