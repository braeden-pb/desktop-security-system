//
// Created by evan on 2026-03-30.
//

#ifndef GROUP55_ALARM_PI_H
#define GROUP55_ALARM_PI_H
#include <string>
#include "NetworkServer.h"
class Alarm_PI {

public:
    Alarm_PI(NetworkServer &server);
    ~Alarm_PI();
    void init();
    void soundAlarm();
    void disableAlarm();

private:
    bool alarmPlaying;
    std::string sound;
    NetworkServer &server;


};


#endif //GROUP55_ALARM_PI_H