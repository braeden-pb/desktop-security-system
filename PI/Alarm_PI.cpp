//
// Created by evan on 2026-03-30.
//

#include "Alarm_PI.h"

Alarm_PI::Alarm_PI(NetworkServer &server) : alarmPlaying(false),server(server) {
    server.onCommand([this](Command cmd, const std::vector<uint8_t>& payload) {
        if (cmd == Command::soundAlarm) {
            if (payload.empty()) {
                std::cerr << "Alarm payload is empty!" << std::endl;
                return;
            }
            sound = std::string(payload.begin(), payload.end());
            std::cout << "Received sound: " << sound << std::endl;

            alarmPlaying = true;
            std::string command = "while true; do aplay -D hw:2,0 /pi/sounds/" + sound + ".wav; done &";
            int result = system(command.c_str());
            if (result != 0) {
                std::cerr << "Failed to start alarm, error: " << result << std::endl;
                alarmPlaying = false;
            }
        } else if (cmd == Command::disableAlarm) {
            disableAlarm();
        }
    });
}

Alarm_PI::~Alarm_PI() {
    disableAlarm();
}

void Alarm_PI::soundAlarm(std::string soundOption) {
    sound = soundOption;
    alarmPlaying = true;
    std::string cmd = "while true; do aplay -D hw:2,0 /pi/sounds/" + sound + ".wav || break; done &";
    int result = system(cmd.c_str());
    if (result != 0) {
        std::cerr << "Failed to start alarm, error: " << result << std::endl;
        alarmPlaying = false;
    }
}




void Alarm_PI::disableAlarm() {
    int result = system(("pkill -f " + sound + ".wav").c_str());
    if (result != 0) {
        std::cerr << "Failed to stop alarm sound, error code: " << result << std::endl;
    }
    alarmPlaying = false;

}
