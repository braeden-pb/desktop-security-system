//
// Created by evan on 2026-03-21.
//

#include "Network.h"

#include <functional>
#include <thread>

Network::Network()
    : socket_(-1)        // -1 means no socket yet
    , connected_(false)  // not connected yet
{
}

Network::~Network() {
    if (connected_) {
        disconnect();
    }
}

bool Network::connect(const std::string &ip, int port) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    if (::connect(socket_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        close(socket_);
        socket_ = -1;
        return false;  // Pi not reachable or not listening
    }
    connected_ = true;
    return true;
}

void Network::disconnect() {
    if (socket_ >= 0) {
        close(socket_);
        socket_ = -1;
    }
    connected_ = false;

}

bool Network::isConnected() const {
    return connected_;
}

void Network::sendRaw(const void* data, size_t size) {
    size_t sent = 0;
    const uint8_t* ptr = (const uint8_t*)data;
    while (sent < size) {
        int n = ::send(socket_, ptr + sent, size - sent, 0);
        if (n <= 0) { connected_ = false; return; }
        sent += n;
    }
}


void Network::send(const PacketHeader& header, const std::vector<uint8_t>& payload) {
    sendRaw(&header, sizeof(header));
    if (!payload.empty()) {
        sendRaw(payload.data(), payload.size());
    }
}

PacketHeader Network::receiveHeader() {
    PacketHeader header;
    // MSG_WAITALL blocks until all bytes arrive
    recv(socket_, &header, sizeof(header), MSG_WAITALL);
    return header;
}

std::vector<uint8_t> Network::receive() {
    PacketHeader header = receiveHeader();

    std::vector<uint8_t> payload(header.payloadSize);
    if (header.payloadSize > 0) {
        recv(socket_, payload.data(), header.payloadSize, MSG_WAITALL);
    }

    return payload;
}

void Network::startReceiving(std::function<void(PacketHeader)> callback) {
    receiveThread = std::thread([this, callback]() {
        while (connected_) {
            PacketHeader header{};
            int n = recv(socket_, &header, sizeof(header), MSG_WAITALL);
            if (n <= 0) {
                connected_ = false;
                break;
            }
            callback(header);
        }
    });
}

void Network::stopReceiving() {
    connected_ = false;
    if (socket_ >= 0) close(socket_);
    if (receiveThread.joinable())
        receiveThread.join();
}
