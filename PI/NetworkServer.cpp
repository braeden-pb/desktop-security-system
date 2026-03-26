//
// Created by evan on 2026-03-21.
//

#include "NetworkServer.h"

NetworkServer::NetworkServer(int port) : port(port) {
    FILE* pipe = popen("hostname -I", "r");
    char buf[64];
    fgets(buf, sizeof(buf), pipe);
    pclose(pipe);
    ip = std::string(buf);
}

NetworkServer::~NetworkServer() {
    stop();
}

bool NetworkServer::start() {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverFd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to bind to port " << port << std::endl;
        return false;
    }

    listen(serverFd, 3);
    running = true;
    acceptThread = std::thread(&NetworkServer::acceptLoop, this);

    std::cout << "Server IP: " << getIP() << std::endl;
    std::cout << "Listening on port " << port << std::endl;
    return true;

}

void NetworkServer::stop() {
    running = false;
    if (clientFd >= 0) close(clientFd);
    if (serverFd >= 0) close(serverFd);
    if (acceptThread.joinable()) acceptThread.join();
    if (receiveThread.joinable()) receiveThread.join();
}

void NetworkServer::onCommand(std::function<void(Command)> callback) {
    commandCallback = callback;
}

void NetworkServer::sendRaw(const void *data, size_t size) {
    size_t sent = 0;
    const uint8_t* ptr = (const uint8_t*)data;

    while (sent < size) {
        int n = send(clientFd, ptr + sent, size - sent, 0);
        if (n <= 0) {
            clientConnected = false;
            return;
        }
        sent += n;
    }
}

void NetworkServer::sendPacket(const PacketHeader& header, const std::vector<uint8_t>& payload) {
    if (!clientConnected) return;
    sendRaw(&header, sizeof(header));
    if (!payload.empty())
        sendRaw(payload.data(), payload.size());
}

void NetworkServer::acceptLoop() {
    while(running) {
        clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0) continue;
        clientConnected = true;
        std::cout << "Client connected!" << std::endl;
        receiveThread = std::thread(&NetworkServer::receiveLoop, this);
        receiveThread.join();
    }
}

void NetworkServer::receiveLoop() {
    while(running && clientConnected) {
        PacketHeader header{};
        int n = recv(clientFd, &header, sizeof(header), MSG_WAITALL);

        if (n <= 0) {
            clientConnected = false;
            break;
        }

        // Fire the callback with the command
        if (commandCallback) {
            commandCallback(header.command);
        }
    }
}

std::string NetworkServer::getIP() {
    return ip;
}

int NetworkServer::getPort() {
    return port;
}
