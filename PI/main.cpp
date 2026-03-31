/**
 * @file main.cpp
 * @brief Entry point for the Raspberry Pi security system application.
 *
 * Initializes and coordinates the NetworkServer, Motion_Sensor_PI, Camera_PI,
 * and Alarm_PI subsystems. Listens for commands from a connected PC client and
 * dispatches them to the appropriate hardware component. Runs until a SIGINT
 * signal (Ctrl+C) is received.
 *
 * @author evan, Micheal a Baeden
 * @date 2026-03-21
 */

#include "Motion_Sensor_PI.h"
#include "NetworkServer.h"
#include <memory>
#include <thread>
#include <chrono>
#include <csignal>
#include "Camera_PI.h"
#include "Alarm_PI.h"

/**
 * @brief Atomic flag indicating whether the application should keep running.
 *
 * Set to false by the signal handler to trigger a graceful shutdown across
 * all threads.
 */
std::atomic<bool> running(true);

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 *
 * Sets the global running flag to false, causing the main loop and the
 * sensor thread to exit cleanly.
 *
 * @param Signal number (unused).
 */

void signalHandler(int) {
    running = false;
}

/**
 * @brief Application entry point.
 *
 * Performs the following startup sequence:
 * -# Registers the SIGINT signal handler for graceful shutdown.
 * -# Starts the NetworkServer on port 5000.
 * -# Constructs and activates the motion sensor, camera, and alarm subsystems.
 * -# Registers a command callback on the server that dispatches incoming PC
 *    commands (stream start/stop, photo, clip, alarm) to the correct subsystem.
 * -# Registers a disconnect callback that stops the camera stream when the
 *    client disconnects.
 * -# Spawns a background sensor thread that polls the running flag and performs
 *    cleanup (deactivate motion sensor, disable alarm, stop server) on exit.
 * -# Blocks the main thread until running is set to false, then joins the
 *    sensor thread and performs a final cleanup.
 *
 * @return 0 on clean exit.
 */

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
