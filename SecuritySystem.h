//
// Created by evan on 2026-02-19.
//
#ifndef GROUP55_SECURITYSYSTEM_H
#define GROUP55_SECURITYSYSTEM_H
#include <string>
#include <memory>

#include "Storage.h"
#include <tuple>

enum class Status : int {
    armed,
    disarmed,
    Error
};

class UI;
class Storage;
class Alarm;

class SecuritySystem
{
public:
    SecuritySystem();
    explicit SecuritySystem(bool headless);
    ~SecuritySystem();
    Status getStatus();
    void arm();
    void disarm();
    bool isArmed() const;
    //getConfig
    std::list<std::tuple<int,std::string, std::string>> getAllImages();
    bool validatePIN(const std::string& pin) const;
    Storage* getStorage() const;
    //getDeviceList
    void soundAlarm();
    bool getIsAlarmActive() const;
    void turnOffAlarm();
    UI* getUI() const;
    Status systemStatus;


private:
    //Config object
    //Devices object list
    std::unique_ptr<Storage> mainStorage;
    std::unique_ptr<UI> mainUi;
    std::unique_ptr<Alarm> alarm;
    void setStatus(Status status);
    void initializeSystem();

};


#endif //GROUP55_SECURITYSYSTEM_H