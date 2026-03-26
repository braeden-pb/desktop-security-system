//
// Created by evan on 2026-03-21.
//

#include "Motion_Sensor_PI.h"
#include "NetworkServer.h"
#include <memory>
#include <thread>
#include <chrono>
#include <csignal>

std::atomic<bool> running(true);



void signalHandler(int) {
    running = false;
}

int main() {
    signal(SIGINT, signalHandler);
    NetworkServer server(5000);
    server.start();

    Motion_Sensor_PI *motion = new Motion_Sensor_PI(5,10);
    motion->activate();

    std::thread sensorThread([&]() {
        while (motion->isActive()) {
            if (motion->isMotionDetected()) {
                PacketHeader header;
                header.system      = System::Motion;
                header.command     = Command::MotionDetected;
                header.payloadSize = 0;
                server.sendPacket(header, {});
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    sensorThread.join();
    motion->deactivate();
    server.stop();
    return 0;

}
