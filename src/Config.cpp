//
// Created by Mitchell on 2026-03-06.
//

#include "Config.h"
using namespace std;

/**
    * @brief Constructs new config object using either saved file values or default values.
    *
    * @details
    * Runs the read from file in order to attempt to read from the config file to initiate the config object
    * using the saved values. If the file does not exist or cannot be read, sends a message to specify this
    * and then initiates config object using simple default values.
    */
Config::Config(Network &network) : network(network) {
    if (readFromFile() == false) { //Attempt to read values from file
        //If the file cannot be read from, specify this and then set default values for all variables.

        std::cout << "Failed to read from file.";
        captureMode = false;
        clipSeconds = 0;
        photosPer = 1;
        password = "123";
        alarmSound = "alarm";
        maxAlarmDuration = 60;
        alarmOnMotion = false;
        writeToFile();
    }
}

/**
     * @brief Deconstructor method.
     */
Config::~Config() {
    authorizedFaces.clear();
}


/**
     * @brief Returns the capture mode.
     *
     * @details
     * Returns the boolean representing the capture mode for the camera.
     *
     * @return The capture mode value.
     */
bool Config::getCaptureMode() {
    return captureMode;
}

/**
     * @brief Sets the capture mode.
     *
     * @details
     * Sets the boolean representing the security system's capture mode to a given boolean value.
     *
     * @param mode The new value for captureMode.
     */
void Config::setCaptureMode(bool mode) {
    captureMode = mode;
}

/**
     * @brief Returns the clip seconds value.
     *
     * @details
     * Returns as an integer the value given for how long the system should record videos in
     * seconds.
     *
     * @return The clip seconds value.
     */
int Config::getSeconds() {
    return clipSeconds;
}

/**
     * @brief Sets the clip seconds value.
     *
     * @details
     * Sets the value representing the number of seconds in a clip to the value given as a
     * parameter.
     *
     * @param seconds The new value for clip seconds.
     */
void Config::setSeconds(int seconds) {
    clipSeconds = seconds;
}

/**
     * @brief Returns the photos per value.
     *
     * @details
     * Returns as an integer the number of photos that are to be taken each time the camera is
     * triggered/an event is called.
     *
     * @return The photos per value.
     */
int Config::getPhotosPer() {
    return photosPer;
}

/**
   * @brief Sets the photos per value.
   *
   * @details
   * Sets the value representing the number of photos taken per event/security system trigger to
   * the value given by this method's parameter.
   *
   * @param photos The new value for photos per.
   */
void Config::setPhotosPer(int photos) {
    photosPer = photos;
}

/**
     * @brief Returns the password.
     *
     * @details
     * Returns a string representing the password being used for this security system.
     *
     * @return A string of the password
     */
string Config::getPassword() {
    return this->password;
}

/**
   * @brief Sets the password value
   *
   * @details
   * Sets the value representing the password for the security system to the given string.
   *
   * @param password The new password to be used.
   */
void Config::setPassword(string password) {
    this->password = password;
}

string Config::getSound() {
    return alarmSound;
}

void Config::setSound(string sound) {
    alarmSound = sound;
}

int Config::getMaxAlarmDuration() {
    return maxAlarmDuration;
}

void Config::setMaxAlarmDuration(int maxAlarmDuration) {
    this->maxAlarmDuration = maxAlarmDuration;
}

bool Config::getAlarmOnMotion() {
    return alarmOnMotion;
}

void Config::setAlarmOnMotion(bool alarmOnMotion) {
    this->alarmOnMotion = alarmOnMotion;
}



/**
     * @brief Stores current config values to file to be read from later.
     *
     * @details
     * Creates or overwrites a file named "config.json". Creates this file with a json format such
     * to store all of the instance variables of the config object. Afterwards, closes the file.
     * If the writing to file is unsuccessful, returns false. Otherwise, returns true upon successful file
     * writing.
     *
     * @return A boolean true if successful, false otherwise.
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
     * @brief Reads value for all config variables from file.
     *
     * @details
     * Checks the "config.json" file that is created by the writeToFile method. Reads out each of
     * the lines in order to set the instance variables for this config object. Order is the same as in
     * the writeToFile method, ending with a comprehensive list of all the entries in the
     * AuthorizedFaces list. If the file does not exist, or the file's format is wrong,
     * or reading of the file otherwise fails, returns false. Returns true upon successful
     * file reading. Appropriately parses json format.
     *
     * @return A boolean true if the file was read without error, false otherwise.
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

