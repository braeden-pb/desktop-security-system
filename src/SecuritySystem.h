//
// Created by evan on 2026-02-19.
//
#ifndef GROUP55_SECURITYSYSTEM_H
#define GROUP55_SECURITYSYSTEM_H
#include <string>
#include <memory>

#include "Storage.h"
#include <tuple>
#include "Observer.h"
#include "../Shared/Protocol.h"



enum class Status : int {
    armed,
    disarmed,
    Error
};

class UI;
class Storage;
class Alarm;
class Config;
class Network;
class Motion_Sensor;
class Camera;

class SecuritySystem : public Observer
{
public:
    void addObserver(Observer* o);
    void notifyObservers(const std::string& event);
    SecuritySystem();
    explicit SecuritySystem(bool headless);
    ~SecuritySystem();
    Status getStatus();
    void arm();
    void disarm();
    bool isArmed() const;
    Config* getConfig();

    void update(const std::string &event);

    void activateHardware();
    std::list<std::tuple<int,std::string, std::string>> getAllImages();
    bool validatePIN(const std::string& pin) const;
    void sendCommand(System sys, Command cmd);
    Storage* getStorage() const;
    Camera* getCamera() const;
    void soundAlarm();
    void soundAlert();
    bool getIsAlarmActive() const;
    void turnOffAlarm() const;
    UI* getUI() const;
    bool isConnected() const;
    void triggerAlert(const std::string& type);
    void setUI(UI* ui);
    Status systemStatus;



private:
    std::unique_ptr<Config> config;
    std::unique_ptr<Network> network;
    std::unique_ptr<Motion_Sensor> motion_sensor;
    std::unique_ptr<Camera> camera;
    std::vector<Observer*> observers;
    std::unique_ptr<Storage> mainStorage;
    std::unique_ptr<UI> mainUi;
    std::unique_ptr<Alarm> alarm;
    void setStatus(Status status);
    void initializeSystem();
    UI* m_ui = nullptr;


};


#endif //GROUP55_SECURITYSYSTEM_H