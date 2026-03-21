//
// Created by evan on 2026-03-21.
//

#include "Network.h"

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

void Network::send(const PacketHeader &header, const std::vector<uint8_t> &payload) {
    ::send(socket_, &header, sizeof(header), 0);

    if (!payload.empty()) {
        ::send(socket_, payload.data(), payload.size(), 0);
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

