//
// Created by evan on 2026-03-26.
//
#include "NetworkServer.h"
#include <iostream>
#include <thread>
#include <chrono>
int main() {
    NetworkServer server(5000);

    server.onCommand([](Command cmd) {
        switch (cmd) {
            case Command::TakePhoto:
                std::cout << "Got command: TakePhoto" << std::endl;
                break;
            case Command::StartClip:
                std::cout << "Got command: StartClip" << std::endl;
                break;
            default:
                std::cout << "Got unknown command" << std::endl;
        }
    });

    server.start();

    // Just keep running
    while (true)
        std::chrono::milliseconds(100);
}