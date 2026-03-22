//
// Created by evan on 2026-03-21.
//

#ifndef GROUP55_DEVICE_H
#define GROUP55_DEVICE_H

#include <string>


class Device {
public:
    Device();
    virtual ~Device();
    virtual std::string getName() const = 0;
    virtual std::string getStatus() const = 0;
    virtual bool connect(const std::string& ip, int port) = 0;
    virtual void disconnect() = 0;
    bool isConnected() const { return connected; }

protected:
    void setConnected(bool state) { connected = state; }

private:
    bool connected = false;
    std::string ip;
    int port;

};


#endif //GROUP55_DEVICE_H