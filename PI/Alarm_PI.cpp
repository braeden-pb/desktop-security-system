//
// Created by evan on 2026-03-30.
//

#include "Alarm_PI.h"

Alarm_PI::Alarm_PI(NetworkServer &server) : alarmPlaying(false),server(server) {

}

Alarm_PI::~Alarm_PI() {
    disableAlarm();
}

void Alarm_PI::soundAlarm() {
    server.onCommand([this](Command cmd, const std::vector<uint8_t>& payload) {
    if (cmd == Command::soundAlarm) {
        std::string option(payload.begin(), payload.end());
        std::cout << "Received: " << option << std::endl;
        sound = option;
        }

    });
    alarmPlaying = true;
   std::string cmd = "while true; do aplay -D hw:2,0 /pi/sounds/"+sound+".wav; done &";
    int result = system(cmd.c_str());
    if (result != 0) {
        std::cerr << "Failed to start alarm sound, error code: " << result << std::endl;
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
