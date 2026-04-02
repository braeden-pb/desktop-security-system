//
// Created by rushd on 3/8/26.
//

#ifndef GROUP55_ALARM_H
#define GROUP55_ALARM_H

#include <string>
#include <SFML/Audio.hpp>
#include "Observer.h"
#include "Network.h"

class Config;

class Alarm : public Observer{
private:
    bool isActive;
    std::string soundType;
    std::string lastActivatedAt;
    sf::SoundBuffer buffer;
    std::optional<sf::Sound> sound;
    Network &network;
    Config &config;
    void notifyObservers(const std::string& event);

public:
    std::vector<Observer*> observers;
    Alarm(Network &network,Config &config);
    ~Alarm();
    void activate();
    void deactivate();
    bool getStatus() const;
    void update(const std::string& event) override;
    void addObserver(Observer* o);
};

#endif // GROUP55_ALARM_H
