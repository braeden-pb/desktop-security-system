/**
* @file Config.cpp
 * @brief Implementation of the Config class for managing persistent security system
 *        settings via a JSON configuration file.
 * @author Mitchell
 * @date 2026-03-06
 */
#include "Config.h"
using namespace std;


/**
 * @brief Constructs a Config object, loading settings from file or applying defaults.
 *
 * Attempts to read configuration values from "config.json" via readFromFile().
 * If the file does not exist or cannot be parsed, falls back to hardcoded default
 * values and immediately writes them to disk via writeToFile() so a valid config
 * file exists for future runs.
 *
 * @param network Reference to the Network instance (reserved for future networked
 *                config synchronization).
 */
Config::Config(Network &network) : network(network) {
    if (readFromFile() == false) { //Attempt to read values from file
        //If the file cannot be read from, specify this and then set default values for all variables.

        std::cout << "Failed to read from file.";
        captureMode = false;
        clipSeconds = 0;
        photosPer = 3;
        photoFreq = 2;
        password = "123";
        alarmSound = "alarm";
        maxAlarmDuration = 60;
        alarmOnMotion = false;
        writeToFile();
    }
}
/**
 * @brief Destructs the Config object.
 *
 * No explicit cleanup required for this version of Config.
 */
Config::~Config() {
}


/**
 * @brief Returns the current capture mode.
 *
 * @return true if photo/video capture is enabled, false otherwise.
 */
bool Config::getCaptureMode() {
    return captureMode;
}
/**
 * @brief Sets the capture mode.
 *
 * @param mode The new capture mode value; true to enable, false to disable.
 */
void Config::setCaptureMode(bool mode) {
    captureMode = mode;
}


/**
 * @brief Returns the clip duration in seconds.
 *
 * @return Integer number of seconds the system records per video clip.
 */
int Config::getSeconds() {
    return clipSeconds;
}
/**
 * @brief Sets the clip duration in seconds.
 *
 * @param seconds The new clip duration value in seconds.
 */
void Config::setSeconds(int seconds) {
    clipSeconds = seconds;
}


/**
 * @brief Returns the number of photos taken per triggered event.
 *
 * @return Integer number of photos captured each time the camera is triggered.
 */
int Config::getPhotosPer() {
    return photosPer;
}

int Config::getPhotoFreq() {
    return photoFreq;
}

void Config::setPhotoFreq(int photoFreq) {
    this->photoFreq = photoFreq;
}

/**
 * @brief Sets the number of photos taken per triggered event.
 *
 * @param photos The new photos-per-event value.
 */
void Config::setPhotosPer(int photos) {
    photosPer = photos;
}


/**
 * @brief Returns the security system password.
 *
 * @return A string containing the current password.
 */
string Config::getPassword() {
    return this->password;
}

/**
 * @brief Sets the security system password.
 *
 * @param password The new password string to use.
 */
void Config::setPassword(string password) {
    this->password = password;
}

/**
 * @brief Returns the alarm sound name.
 *
 * @return A string containing the configured alarm sound name.
 */
string Config::getSound() {
    return alarmSound;
}
/**
 * @brief Sets the alarm sound name.
 *
 * @param sound The name of the alarm sound to use.
 */
void Config::setSound(string sound) {
    alarmSound = sound;
}
/**
 * @brief Returns the maximum alarm duration in seconds.
 *
 * @return Integer number of seconds the alarm is allowed to sound before
 *         automatically stopping.
 */
int Config::getMaxAlarmDuration() {
    return maxAlarmDuration;
}
/**
 * @brief Sets the maximum alarm duration in seconds.
 *
 * @param maxAlarmDuration The new maximum alarm duration value in seconds.
 */
void Config::setMaxAlarmDuration(int maxAlarmDuration) {
    this->maxAlarmDuration = maxAlarmDuration;
}
/**
 * @brief Returns whether the alarm should trigger automatically on motion detection.
 *
 * @return true if the alarm fires on motion, false otherwise.
 */
bool Config::getAlarmOnMotion() {
    return alarmOnMotion;
}
/**
 * @brief Sets whether the alarm should trigger automatically on motion detection.
 *
 * @param alarmOnMotion true to enable alarm-on-motion, false to disable.
 */
void Config::setAlarmOnMotion(bool alarmOnMotion) {
    this->alarmOnMotion = alarmOnMotion;
}




/**
 * @brief Serializes all configuration values to "config.json".
 *
 * Creates or overwrites "config.json" in the working directory, writing all
 * instance variables in JSON format. Any exception during file I/O causes the
 * method to return false immediately.
 *
 * @return true if the file was written successfully, false on any I/O error.
 */
bool Config::writeToFile() {
    try {
        ofstream configFile("config.json");
        if (!configFile.is_open()) {
            std::cerr << "test" << std::endl;
            return false;
        }

        configFile << "{\n";
        configFile << "\t\"captureMode\": \"" << captureMode << "\"," << endl;
        configFile << "\t\"clipSeconds\": \"" << clipSeconds << "\"," << endl;
        configFile << "\t\"photosPer\": \"" << photosPer << "\"," << endl;
        configFile << "\t\"photosFreq\": \"" << photoFreq << "\"," << endl;
        configFile << "\t\"password\": \"" << password << "\"," << endl;
        configFile << "\t\"sound\": \"" << alarmSound << "\"," << endl;
        configFile << "\t\"MaxDuration\": \"" << maxAlarmDuration << "\"," << endl;
        configFile << "\t\"AlarmOnMotion\": \"" << alarmOnMotion << "\"" << endl;
        configFile << "}";

        configFile.close();
    } catch (...) { //If any error is caught, return false.
        return false;
    }

    return true; //Return true if no errors
}


/**
 * @brief Deserializes all configuration values from "config.json".
 *
 * Reads "config.json" line by line in the same order as writeToFile(), parsing
 * each quoted value after the colon separator. Any exception during parsing or
 * file I/O causes the method to return false immediately.
 *
 * @return true if the file was read and all values parsed successfully,
 *         false if the file is missing, malformed, or any I/O error occurs.
 *
 * @note The field order in the file must exactly match the order written by
 *       writeToFile(); any deviation will cause parsing to fail or produce
 *       incorrect values.
 */
bool Config::readFromFile() {
    try {
        string inText;
        ifstream configFile("config.json");

        getline(configFile, inText);



        getline(configFile, inText);
        auto start = inText.find("\"", inText.find(":")) + 1;
        auto end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        if (inText == "1")
            captureMode = true;
        else
            captureMode = false;

        getline(configFile, inText);
        start = inText.find("\"", inText.find(":")) + 1;
        end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        this->clipSeconds = stoi(inText);

        getline(configFile, inText);
        start = inText.find("\"", inText.find(":")) + 1;
        end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        this->photosPer = stoi(inText);

        getline(configFile, inText);
        start = inText.find("\"", inText.find(":")) + 1;
        end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        this->photoFreq = stoi(inText);

        getline(configFile, inText);
        start = inText.find("\"", inText.find(":")) + 1;
        end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        this->password = inText;

        getline(configFile, inText);
        start = inText.find("\"", inText.find(":")) + 1;
        end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        this->alarmSound = inText;

        getline(configFile, inText);
        start = inText.find("\"", inText.find(":")) + 1;
        end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        this->maxAlarmDuration = stoi(inText);

        getline(configFile, inText);
        start = inText.find("\"", inText.find(":")) + 1;
        end = inText.find("\"", start);
        inText = inText.substr(start, end - start);
        this->alarmOnMotion = stoi(inText);

    } catch (...) { //If any error occurs, return false.
        return false;
    }

    return true; //Return true if no errors
}
