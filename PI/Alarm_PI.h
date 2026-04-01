//
// Created by evan on 2026-03-30.
//

#ifndef GROUP55_ALARM_PI_H
#define GROUP55_ALARM_PI_H
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "NetworkServer.h"

class Alarm_PI {

public:
    Alarm_PI(NetworkServer &server);
    ~Alarm_PI();
    void init();
    void soundAlarm(std::string soundOption);
    void disableAlarm();

private:
    bool alarmPlaying;
    std::string sound;
    NetworkServer &server;
    pid_t alarmPid = -1;


};


#endif //GROUP55_ALARM_PI_H
