//
// Created by evan on 2026-03-24.
//

#ifndef GROUP55_MOTION_SENSOR_PI_H
#define GROUP55_MOTION_SENSOR_PI_H

#include <wiringPi.h>
#include "NetworkServer.h"
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>

class Motion_Sensor_PI {
    public:
    Motion_Sensor_PI(int sensitivity, int motionSleep,NetworkServer &network);
    ~Motion_Sensor_PI();
    static void isrHandler();
    void activate();
    void deactivate();
    bool isActive();
    bool isMotionDetected();




private:
    std::atomic<bool> motionPending = false;
    time_t lastSent = 0;
    void detectMotion();
    void onMotion();

    std::atomic<bool> active;
    std::atomic<bool> motionDetected;
    NetworkServer& network;
    int sensitivity;
    void setSensitivity(int sensitivity);
    int motionSleep;

    static Motion_Sensor_PI* instance_;
    std::thread sensorThread;
    std::mutex mtx;
};


#endif //GROUP55_MOTION_SENSOR_PI_H
