#include "Network.h"
#include "../Shared/Protocol.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Network client;

    std::cout << "Connecting to Pi..." << std::endl;

    if (!client.connect("10.42.0.205", 5000)) {
        std::cerr << "Failed to connect" << std::endl;
        return 1;
    }

    std::cout << "Connected! Sending commands..." << std::endl;

    // Send TakePhoto
    PacketHeader header;
    header.system      = System::Camera;
    header.command     = Command::TakePhoto;
    header.payloadSize = 0;
    client.send(header, {});
    std::cout << "Sent TakePhoto" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Send StartClip
    header.command = Command::StartClip;
    client.send(header, {});
    std::cout << "Sent StartClip" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    client.disconnect();
    std::cout << "Done" << std::endl;
}