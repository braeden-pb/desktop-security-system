/**
* @file Alarm_PI.cpp
 * @brief Implementation of the Alarm_PI class for managing alarm sounds on Raspberry Pi.
 * @author evan, Rushd
 * @date 2026-03-30
 */
#include "Alarm_PI.h"

/**
 * @brief Constructs an Alarm_PI object.
 *
 * Initializes the alarm system with a reference to the network server.
 * The alarm is set to not playing by default.
 *
 * @param server Reference to the NetworkServer instance used for network communication.
 */

Alarm_PI::Alarm_PI(NetworkServer &server) : alarmPlaying(false),server(server) {

}

/**
 * @brief Destructs the Alarm_PI object.
 *
 * Ensures any currently playing alarm is stopped before the object is destroyed.
 */

Alarm_PI::~Alarm_PI() {
    disableAlarm();
}

/**
 * @brief Starts playing an alarm sound.
 *
 * Forks a child process to play the specified WAV audio file using the `play` command
 * (from the SoX audio toolkit). If an alarm is already playing, it is stopped before
 * the new one starts. The audio is directed to the hardware device `plughw:2,0`.
 * The sound plays on an infinite loop until explicitly stopped via disableAlarm().
 *
 * @param soundOption The name of the sound file (without extension) to play.
 *                    The file is expected to be located in the `../sounds/` directory.
 *
 * @note Uses fork() and execl() to spawn a child process running `play`.
 * @warning If fork() or execl() fails, an error is printed to stderr and the alarm
 *          will not play.
 */

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


/**
 * @brief Stops the currently playing alarm.
 *
 * Sends SIGKILL to the child process responsible for audio playback and waits
 * for it to terminate. Resets the alarm state and clears the sound name.
 * Does nothing if no alarm is currently playing.
 *
 * @note Uses kill() and waitpid() to terminate and reap the child process.
 */

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
