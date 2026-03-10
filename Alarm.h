//
// Created by rushd on 3/8/26.
//

#ifndef GROUP55_ALARM_H
#define GROUP55_ALARM_H

#include <string>
#include <SFML/Audio.hpp>


class Alarm {
private:
    bool isActive;                 // shows whether the alarm is currently active
    int volume;                    // alarm volume level
    std::string soundType;         // type of sound the alarm produces
    std::string lastActivatedAt;   // timestamp of the last activation

public:
    // Constructor
    Alarm();

    // Activates the alarm indefinitely
    void activate();

    bool loadSound(sf::SoundBuffer& buffer);  // bool not sf::SoundBuffer
    // Deactivates the alarm
    void deactivate();

    // Tests the alarm sound
    void testAlarm();

    // Returns the current alarm status
    bool getStatus() const;
};

#endif // GROUP55_ALARM_H