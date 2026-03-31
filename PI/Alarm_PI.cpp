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
    if (alarmPlaying) disableAlarm();
    sound = soundOption;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }
    if (pid == 0) {
        setenv("AUDIODEV", "plughw:2,0", 1);
        execl("/usr/bin/play", "play",
              ("../sounds/" + sound + ".wav").c_str(),
              "repeat", "-", nullptr);
        perror("execl failed");
        exit(1);
    }

    alarmPid = pid;
    alarmPlaying = true;
}




void Alarm_PI::disableAlarm() {
    if (!alarmPlaying) return;
    if (alarmPid > 0) {
        kill(alarmPid, SIGKILL);
        waitpid(alarmPid, nullptr, 0);
        alarmPid = -1;
    }
    alarmPlaying = false;
    sound.clear();

}
