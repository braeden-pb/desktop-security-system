//
// Created by rushd on 3/8/26.
//

#ifndef GROUP55_ALARM_H
#define GROUP55_ALARM_H

#include <string>
#include <SFML/Audio.hpp>
#include "Observer.h"


class Alarm : public Observer{
private:
    bool isActive;                 // shows whether the alarm is currently active
    int volume;                    // alarm volume level
    std::string soundType;         // type of sound the alarm produces
    std::string lastActivatedAt;   // timestamp of the last activation

    sf::SoundBuffer buffer;
    std::optional<sf::Sound> sound;

public:
    // Constructor
    Alarm();

    // Activates the alarm indefinitely
    void activate();

    virtual bool loadSound(sf::SoundBuffer& buffer);  // bool not sf::SoundBuffer
    // Deactivates the alarm
    void deactivate();

    // Tests the alarm sound
    void testAlarm();

    // Returns the current alarm status
    bool getStatus() const;

    void update(const std::string& event) override;
};

#endif // GROUP55_ALARM_H
