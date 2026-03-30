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
#include <fcntl.h>
#include <sys/select.h>
#include <map>
#include <iostream>
#include "../Shared/Protocol.h"


class Network {

public:
    Network();
    ~Network();
    bool connect(const std::string& ip, int port);
    void disconnect();
    bool isConnected() const;


    void send(const PacketHeader& header, const std::vector<uint8_t>& payload);
    std::vector<uint8_t> receive();

    void onPacket(System system,
                  std::function<void(Command, const std::vector<uint8_t>&)> callback);

    void startReceiving();

    void stopReceiving();

    PacketHeader receiveHeader();

private:
    int socket_ = -1;
    std::atomic<bool> connected_ = false;
    std::thread receiveThread;
    void sendRaw(const void *data, size_t size);
    void receiveLoop();
    std::map<System, std::function<void(Command, const std::vector<uint8_t>&)>> handlers;


};


#endif //GROUP55_NETWORK_H