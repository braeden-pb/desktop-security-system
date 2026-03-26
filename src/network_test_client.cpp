//
// Created by evan on 2026-03-26.
//

// pc/test_client.cpp
#include "Network.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Network client;

    std::cout << "Connecting to Pi..." << std::endl;

    if (!client.connect("192.168.2.1", 5000)) {
        std::cerr << "Failed to connect" << std::endl;
        return 1;
    }

    std::cout << "Connected! Sending commands..." << std::endl;

    // Send a photo command
    PacketHeader header{Command::TakePhoto, 0};
    client.send(header, {});
    std::cout << "Sent TakePhoto" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Send a clip command
    header.command = Command::StartClip;
    client.send(header, {});
    std::cout << "Sent StartClip" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    client.disconnect();
    std::cout << "Done" << std::endl;
}