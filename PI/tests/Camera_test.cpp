//
// Created by evan on 2026-03-27.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "../Camera_PI.h"
#include "../NetworkServer.h"

int main() {
    // Start network server first
    NetworkServer server(8080);
    if (!server.start()) {
        std::cerr << "Failed to start server." << std::endl;
        return 1;
    }

    std::cout << "Waiting for PC to connect..." << std::endl;
    while (!server.isClientConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "PC connected." << std::endl;

    Camera_PI camera(server);

    // Test 1 — take a photo
    std::cout << "\n--- Testing capturePhoto ---" << std::endl;
    std::string path = camera.capturePhoto();
    if (!path.empty())
        std::cout << "capturePhoto OK: " << path << std::endl;
    else
        std::cerr << "capturePhoto FAILED" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));


    // Test 3 — stream (press Enter to stop)
    std::cout << "\n--- Testing streamVideo (press Enter to stop) ---" << std::endl;
    camera.streamVideo();

    return 0;
}