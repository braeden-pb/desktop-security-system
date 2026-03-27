//
// Created by evan on 2026-03-27.
//

// pc_test.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../Shared/Protocol.h"

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8080);
    inet_pton(AF_INET, "192.168.2.174", &addr.sin_addr);

    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to connect." << std::endl;
        return 1;
    }
    std::cout << "Connected to Pi." << std::endl;

    int fileCount = 0;
    while (true) {
        // Read header
        PacketHeader header{};
        int n = recv(sock, &header, sizeof(header), MSG_WAITALL);
        if (n <= 0) break;

        // Read payload
        std::vector<uint8_t> payload(header.payloadSize);
        if (header.payloadSize > 0)
            recv(sock, payload.data(), header.payloadSize, MSG_WAITALL);

        if (header.command == Command::TakePhoto) {
            std::string filename = "photo_" + std::to_string(fileCount++) + ".jpg";
            std::ofstream f(filename, std::ios::binary);
            f.write(reinterpret_cast<const char*>(payload.data()), payload.size());
            std::cout << "Photo received → " << filename
                      << " (" << payload.size() << " bytes)" << std::endl;
        } else if (header.command == Command::Frame) {
            std::cout << "Frame received: " << payload.size() << " bytes" << std::endl;
        }
    }

    close(sock);
    return 0;
}