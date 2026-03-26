//
// Created by evan on 2026-03-21.
//

#ifndef GROUP55_NETWORKSERVER_H
#define GROUP55_NETWORKSERVER_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>
#include <atomic>
#include <unistd.h>
#include <iostream>
#include "../Shared/Protocol.h"
#include <thread>


class NetworkServer {
    public:
        NetworkServer(int port);
        ~NetworkServer();
        bool start();
        void stop();
        std::string getIP();
        int getPort();
        void onCommand(std::function<void(Command)> callback);
        bool isClientConnected() const { return clientConnected; }

    private:
        void acceptLoop();
        void receiveLoop();
        void sendRaw(const void* data, size_t size);
        int port;
        std::string ip;
        int serverFd = -1;
        int clientFd = -1;
        std::thread acceptThread;
        std::thread receiveThread;
        std::function<void(Command)> commandCallback;
        std::atomic<bool> running{false};
        std::atomic<bool> clientConnected{false};

};


#endif //GROUP55_NETWORKSERVER_H