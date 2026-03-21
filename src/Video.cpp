/**
* @file Video.cpp
 * @brief Implementation of the Video class, specializing File for media playback.
 * @author Braeden Patierno-Barker
 * @date 3/6/2026
 */

#include "Video.h"

/**
 * @brief Construct a new Video object.
 * * @param id Unique identifier.
 * @param filePath Path to the video file.
 * @param timeStamp Metadata timestamp.
 * @param resolution Video dimensions.
 * @param videoLength Formatted duration string.
 * @param isPlaying Initial playback state.
 * @param player Pointer to the wxMediaCtrl instance used for rendering.
 */
Video::Video(int id, std::string filePath, std::string timeStamp, std::string resolution, std::string videoLength, bool isPlaying, wxMediaCtrl* player)
    : File(id, filePath, timeStamp, resolution), videoLength(videoLength), isPlaying(false), player(player) {
}

//! Default destructor.
Video::~Video() {}

/**
 * @brief Starts or resumes video playback.
 * * Loads the file into the wxMediaCtrl if not already loaded and triggers
 * the Play() command.
 */
void Video::play() {
    // Ensure the media control exists and we aren't already in a playing state
    if (player && !isPlaying) {
        if (player->Load(filePath)) {
            player->Play();
            isPlaying = true;
        }
    }
}

/**
 * @brief Pauses the current video playback.
 * * Updates the internal state to reflect that the media is no longer active.
 */
void Video::pause() {
    if (player && isPlaying) {
        player->Pause();
        isPlaying = false;
    }
}

/**
 * @brief Retrieves the total duration of the video.
 * @return std::string The formatted length (e.g. "00:05:30").
 */
std::string Video::getDuration() {
    return videoLength;
}