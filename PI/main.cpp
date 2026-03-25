//
// Created by evan on 2026-03-21.
//

#include "Motion_Sensor_PI.h"

int main() {
    Motion_Sensor_PI *motion = new Motion_Sensor_PI(5,10);
    motion->activate();
    while (motion->isActive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    motion->deactivate();
    return 0;

}
