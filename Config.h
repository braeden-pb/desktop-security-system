//
// Created by Mitchell on 2026-03-06.
//

#ifndef GROUP55_CONFIG_H
#define GROUP55_CONFIG_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Config {
public:
    /**
     * @brief Constructs new config object using either saved file values or default values.
     *
     * @details
     * Runs the read from file in order to attempt to read from the config file to initiate the config object
     * using the saved values. If the file does not exist or cannot be read, sends a message to specify this
     * and then initiates config object using simple default values.
     */
    Config();

    /**
     * @brief Deconstructor method.
     */
    ~Config();

    /**
  * @brief Returns the motion sensitivity.
  *
  * @details
  * Returns as an integer the value given to model the sensitivity for the security system's
  * motion detection.
  *
  * @return The motion sensitivity value.
  */
    int getSensitivity();

    /**
     * @brief Sets the motion sensitivity.
     *
     * @details
     * Sets the value representing the security system's motion sensitivity to whatever integer
     * is given as a parameter.
     *
     * @param sensitivity The new value for motion sensitivity.
     */
    void setSensitivity(int sensitivity);

   /**
     * @brief Returns the capture mode.
     *
     * @details
     * Returns the boolean representing the capture mode for the camera.
     *
     * @return The capture mode value.
     */
    bool getCaptureMode();

   /**
     * @brief Sets the capture mode.
     *
     * @details
     * Sets the boolean representing the security system's capture mode to a given boolean value.
     *
     * @param mode The new value for captureMode.
     */
    void setCaptureMode(bool mode);

   /**
     * @brief Returns the clip seconds value.
     *
     * @details
     * Returns as an integer the value given for how long the system should record videos in
     * seconds.
     *
     * @return The clip seconds value.
     */
    int getSeconds();

   /**
     * @brief Sets the clip seconds value.
     *
     * @details
     * Sets the value representing the number of seconds in a clip to the value given as a
     * parameter.
     *
     * @param seconds The new value for clip seconds.
     */
    void setSeconds(int seconds);

   /**
     * @brief Returns the photos per value.
     *
     * @details
     * Returns as an integer the number of photos that are to be taken each time the camera is
     * triggered/an event is called.
     *
     * @return The photos per value.
     */
    int getPhotosPer();

   /**
   * @brief Sets the photos per value.
   *
   * @details
   * Sets the value representing the number of photos taken per event/security system trigger to
   * the value given by this method's parameter.
   *
   * @param photos The new value for photos per.
   */
    void setPhotosPer(int photos);

   /**
   * @brief Returns the full list of authorized faces.
   *
   * @details
   * Returns a vector of strings which stores all of the authorized faces for the security system.
   * Each face is stored as a string containing a code that can be read by the facial recognition
   * class.
   *
   * @return The full vector of authorized faces.
   */
    vector<string> getAuthorizedFaces();

    /**
     * @brief Removes a given face from the list of authorized faces.
     *
     * @details
     * When given a string, removes this string from the authorized faces list, should it exist.
     * If the value cannot be found and removed in the list, returns false. If the item is
     * successfully removed, returns true.
     *
     * @param face The string representing the face value to be removed.
     * @return A boolean true if successful, false otherwise.
     */
    bool removeAuthorizedFace(string face);

    /**
     * @brief Adds a new face to the list of authorized faces.
     *
     * @details
     * Given a string representation of a face, adds this face to the list of authorized faces for
     * the security system. If the face already exists, or the operation is otherwise unsuccessful,
     * returns false. Returns true if this face is successfully added, and did not already exist.
     *
     * @param face The string representation of the face added.
     *
     * @return A boolean true if successful, false otherwise.
     */
    bool addAuthorizedFace(string face);

    /**
     * @brief Returns whether a given face is authorized.
     *
     * @details
     * When given a string representation of a face, iterates through the list of authorized
     * faces in order to determine if this face exists in the list. If it does, returns true.
     * Otherwise, returns false.
     *
     * @param face The face being checked.
     * @return A boolean true if this face is authorized. False otherwise.
     */
    bool isAuthorizedFace(string face);

    /**
     * @brief Clears authorized faces list.
     *
     * @details
     * When called, this method will clear the authorized faces list, fully resetting the list.
     * Removes all stored string values in the list.
     */
    void resetAuthorizedFaces();

    /**
     * @brief Stores current config values to file to be read from later.
     *
     * @details
     * Creates or overwrites a file named "config.txt". The first line of this file will contain the
     * value of motionSensitivty. Second line contains captureMode, either 1 or 0. Third line contains
     * clipSeconds. Fourth line contains photosPer. After this, each following line contains one entry
     * from the list of authorized faces, until the entire list is written out. Afterwards, closes the file.
     * If the writing to file is unsuccessful, returns false. Otherwise, returns true upon successful file
     * writing.
     *
     * @return A boolean true if successful, false otherwise.
     */
    bool writeToFile();

private:
    /**
     * Stores the sensitivity level for the motion detection as an integer value.
     */
    int motionSensitivity;

    /**
     * Stores the capture mode for the security system as a boolean value.
     */
    bool captureMode;

    /**
     * Stores the number of seconds per clip recorded as an integer value.
     */
    int clipSeconds;

    /**
     * Stores the number of photos taken per camera trigger as an integer value.
     */
    int photosPer;

    /**
     * Stores a list of strings representing all the authorized faces for the security system.
     */
    vector<string> authorizedFaces;

    /**
     * @brief Reads value for all config variables from file.
     *
     * @details
     * Checks the "config.txt" file that is created by the writeToFile method. Reads out each of
     * the lines in order to set the instance variables for this config object. Order is the same as in
     * the writeToFile method, ending with a comprehensive list of all the entries in the
     * AuthorizedFaces list. If the file does not exist, or the file's format is wrong,
     * or reading of the file otherwise fails, returns false. Returns true upon successful
     * file reading.
     *
     * @return A boolean true if the file was read without error, false otherwise.
     */
    bool readFromFile();
};


#endif //GROUP55_CONFIG_H