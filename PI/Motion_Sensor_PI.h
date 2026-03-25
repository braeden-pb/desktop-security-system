//
// Created by evan on 2026-03-24.
//

#ifndef GROUP55_MOTION_SENSOR_PI_H
#define GROUP55_MOTION_SENSOR_PI_H

#include <wiringPi.h>
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>

class Motion_Sensor_PI {
    public:
    Motion_Sensor_PI(int sensitivity, int motionSleep);
    ~Motion_Sensor_PI();
    static void isrHandler();
    void activate();
    void deactivate();
    bool isActive();



private:
    void detectMotion();
    void onMotion();
    std::atomic<bool> active;
    std::atomic<bool> motionDetected;
    int sensitivity;
    void setSensitivity(int sensitivity);
    int motionSleep;

    static Motion_Sensor_PI* instance_;
    std::thread sensorThread;
    std::mutex mtx;
};


#endif //GROUP55_MOTION_SENSOR_PI_H