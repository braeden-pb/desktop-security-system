//
// Created by evan on 2026-03-21.
//

#ifndef GROUP55_NETWORK_H
#define GROUP55_NETWORK_H

#include <cstdint>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../Protocol.h"


class Network {

public:
    Network();
    ~Network();
    bool connect(const std::string& ip, int port);
    void disconnect();
    bool isConnected() const;
    void send(const PacketHeader& header, const std::vector<uint8_t>& payload);
    std::vector<uint8_t> receive();
    PacketHeader receiveHeader();

private:
    int socket_ = -1;
    bool connected_ = false;

};


#endif //GROUP55_NETWORK_H