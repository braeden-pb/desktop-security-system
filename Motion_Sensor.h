//
// Created by Braeden Patierno-Barker on 3/22/2026.
//

#ifndef GROUP55_MOTION_SENSOR_H
#define GROUP55_MOTION_SENSOR_H

#include "src/Device.h"
#include <string>
#include <chrono>
#include <ctime>

class Motion_Sensor : public Device {
    private:
        int sensitivity;
        bool motionDetected;
        std::time_t lastDetected;
        bool lastState;
        bool rearmPending;
        int motionCount;
        int rearmDelayMs;
        std::chrono::steady_clock::time_point rearmUntil;

    public:
        Motion_Sensor();
        ~Motion_Sensor();
        std::string getName() const override;
        std::string getStatus() const override;
        bool connect(const std::string& ip, int port) override;
        void disconnect() override;
        void updateState(bool currentState);
        bool detectMotion();
        void onMotion();
        void updateStatus();
        int getSensitivity();
        void setSensitivity(int newSensitivity);
};


#endif //GROUP55_MOTION_SENSOR_H