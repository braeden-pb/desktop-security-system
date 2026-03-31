//
// Created by evan on 2026-03-21.
//

#include "Motion_Sensor_PI.h"
#include "NetworkServer.h"
#include <memory>
#include <thread>
#include <chrono>
#include <csignal>
#include "Camera_PI.h"
#include "Alarm_PI.h"

std::atomic<bool> running(true);

void signalHandler(int) {
    running = false;
}

int main() {
    signal(SIGINT, signalHandler);

    NetworkServer server(5000);
    server.start();

    Motion_Sensor_PI motion(5, 10, server);
    Camera_PI camera(server);
    Alarm_PI alarm(server);

    motion.activate();

    server.onCommand([&camera,&alarm](Command cmd,const std::vector<uint8_t>& payload) {
        switch (cmd) {
            case Command::StartStream: camera.startStreaming(); break;
            case Command::StopStream:  camera.stopStreaming();  break;
            case Command::TakePhoto:   camera.capturePhoto();   break;
            case Command::StartClip:   camera.startRecording(); break;
            case Command::StopClip:    camera.stopRecording();  break;
            case Command::disableAlarm: alarm.disableAlarm();    break;
            case Command::soundAlarm:   alarm.soundAlarm(std::string(payload.begin(), payload.end())); break;
            default: break;
        }
    });

    server.onDisconnect([&camera]() {
        camera.stopStreaming();
    });

    std::thread sensorThread([&]() {
        while (running) {
       std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }

   motion.deactivate();
        alarm.disableAlarm();
   server.stop();
   return 0;
    });

    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    sensorThread.join();
    motion.deactivate();
    server.stop();
    return 0;
}
