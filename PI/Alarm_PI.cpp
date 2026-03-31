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
    alarmPlaying = true;
    std::string cmd = "while true; do aplay -D hw:2,0 ../sounds/" + sound + ".wav || break; done &";
    int result = system(cmd.c_str());
    if (result != 0) {
        std::cerr << "Failed to start alarm, error: " << result << std::endl;
        alarmPlaying = false;
    }
}




void Alarm_PI::disableAlarm() {
    std::string cmd = "pkill -f \"aplay.*" + sound + "\"";
    system(cmd.c_str());
    // Also kill any lingering shell loops
    system("pkill -f 'while true'");
    //int result = system(("pkill -f " + "../sounds" + sound + ".wav").c_str());
    // if (result != 0) {
    //     std::cerr << "Failed to stop alarm sound, error code: " << result << std::endl;
    // }
    alarmPlaying = false;

}
