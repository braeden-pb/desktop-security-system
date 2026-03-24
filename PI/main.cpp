//
// Created by evan on 2026-03-21.
//

#include "Motion_Sensor_PI.h"

int main(int argc, char *argv[]) {
    Motion_Sensor_PI *motion = new Motion_Sensor_PI(5,5);
    motion->activate();

}
