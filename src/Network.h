//
// Created by evan on 2026-03-21.
//

#ifndef GROUP55_NETWORK_H
#define GROUP55_NETWORK_H

#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../Shared/Protocol.h"


class Network {

public:
    Network();
    ~Network();
    bool connect(const std::string& ip, int port);
    void disconnect();
    bool isConnected() const;

    void sendRaw(const void *data, size_t size);

    void send(const PacketHeader& header, const std::vector<uint8_t>& payload);
    std::vector<uint8_t> receive();

    void startReceiving(std::function<void(PacketHeader)> callback);

    void stopReceiving();

    PacketHeader receiveHeader();

private:
    int socket_ = -1;
    bool connected_ = false;
    std::thread receiveThread;

};


#endif //GROUP55_NETWORK_H