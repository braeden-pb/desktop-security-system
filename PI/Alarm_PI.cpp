//
// Created by evan on 2026-03-30.
//

#include "Alarm_PI.h"

Alarm_PI::Alarm_PI(NetworkServer &server) : alarmPlaying(false),server(server) {

}

Alarm_PI::~Alarm_PI() {
    disableAlarm();
}

void Alarm_PI::soundAlarm(std::string soundOption) {
    sound = soundOption;
    pid_t pid = fork();
    if (pid == 0) {
        execl("/usr/bin/aplay", "aplay", "-D", "plughw:2,0",
              ("../sounds/" + sound + ".wav").c_str(), nullptr);
        exit(0);
    }
    // Save pid to kill later
    alarmPid = pid;
    alarmPlaying = true;
}




void Alarm_PI::disableAlarm() {
    if (sound.empty()) return;
     if (alarmPid > 0) {
        kill(alarmPid, SIGKILL);
        alarmPid = -1;
    }
    alarmPlaying = false;

}
