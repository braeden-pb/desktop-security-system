/**
 * @file Camera.cpp
 * @brief Implementation of the Camera class for sending camera commands to the
 *        Raspberry Pi and handling incoming frame, photo, and video packets on the PC side.
 * @author evan, Micheal a Baeden
 * @date 2026-03-21
 */
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <filesystem>

#include "Storage.h"
/**
 * @brief Constructs a Camera object and registers a packet handler for camera events.
 *
 * Registers a Network packet callback for System::Camera packets so that
 * incoming Frame, TakePhoto, and StartClip responses from the Pi are automatically
 * routed to handlePacket(). Also stores a reference to Storage for persisting
 * captured media metadata.
 *
 * @param network Reference to the Network instance used to send commands
 *                and receive packets from the Raspberry Pi.
 * @param storage Reference to the Storage instance used to record saved
 *                photo and video file paths with their timestamps.
 */
Camera::Camera(Network &network,Storage &storage) : recording(false), network(network),storage(storage) {
    network.onPacket(System::Camera, [this](Command cmd, const std::vector<uint8_t> &payload) {
        handlePacket(cmd, payload);
    });
}

/**
 * @brief Destructs the Camera object and stops any active stream or recording.
 *
 * Ensures stopStream() and stopRecording() are called if either is still active,
 * sending the appropriate stop commands to the Pi before the object is destroyed.
 */
Camera::~Camera() {
    if (streaming) stopStream();
}

/**
 * @brief Sends a photo capture request to the Raspberry Pi.
 *
 * Transmits a TakePhoto command packet with no payload. The actual image
 * is received asynchronously via handlePacket() when the Pi responds,
 * at which point lastPhoto is populated and the photoCallback is fired.
 *
 * @return The file path of the most recently saved photo. May be empty if
 *         no photo response has been received yet.
 */
std::string Camera::capturePhoto() {
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::TakePhoto;
    header.payloadSize = 0;
    network.send(header, {});
    return lastPhoto; // will be populated when Pi responds
}
/**
 * @brief Sends a start stream command to the Raspberry Pi.
 *
 * Transmits a StartStream packet and sets the local streaming flag to true.
 * Incoming JPEG frames will subsequently arrive via handlePacket() and be
 * forwarded to the registered frame callback.
 *
 * @note Does nothing if streaming is already active.
 */
void Camera::startStream() {
    if (streaming) return;
    std::cout << "Sending StartStream command..." << std::endl;
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::StartStream;
    header.payloadSize = 0;
    network.send(header, {});
    streaming = true;
}
/**
 * @brief Sends a stop stream command to the Raspberry Pi.
 *
 * Transmits a StopStream packet and clears the local streaming flag.
 *
 * @note Does nothing if streaming is not currently active.
 */
void Camera::stopStream() {
    if (!streaming) return;
    PacketHeader header{};
    header.system      = System::Camera;
    header.command     = Command::StopStream;
    header.payloadSize = 0;
    network.send(header, {});
    streaming = false;
}

/**
 * @brief Returns whether the camera is currently streaming.
 *
 * @return true if a stream is active, false otherwise.
 */
bool Camera::isStreaming() const {
    return streaming;
}

/**
 * @brief Handles an incoming camera packet dispatched by the Network layer.
 *
 * Processes three packet types:
 * - **Frame**: Stores the JPEG payload as the latest frame under mutex protection,
 *   sets the newFrameAvailable flag, and fires the frameCallback if registered.
 * - **TakePhoto**: Saves the JPEG payload to a timestamped .jpg file under
 *   ../saved_data/, registers the path and formatted timestamp with Storage,
 *   and fires the photoCallback if registered.
 * - **StartClip**: Saves the raw MJPEG payload to a timestamped .mjpeg file under
 *   ../saved_data/ and registers the path and formatted timestamp with Storage.
 *
 * Unrecognized commands are logged and ignored.
 *
 * @param cmd     The command type extracted from the incoming PacketHeader.
 * @param payload The raw byte payload accompanying the packet.
 *
 * @note Frame storage is protected by frameMutex to allow safe access from
 *       the UI or rendering thread via getLatestFrame() and hasNewFrame().
 */
void Camera::handlePacket(Command cmd, const std::vector<uint8_t> &payload) {
    //std::cout << "Packet received, command: " << static_cast<int>(cmd) << std::endl;
    switch (cmd) {
        case Command::Frame: {
            //std::cout << "Frame packet: " << payload.size() << " bytes" << std::endl;
            {
                std::lock_guard<std::mutex> lock(frameMutex);
                latestFrame       = payload;
                newFrameAvailable = true;
            }
            if (frameCallback)
                frameCallback(payload);
            break;
        }
        case Command::TakePhoto: {
            std::cout << "Photo packet: " << payload.size() << " bytes" << std::endl;
            if (payload.empty()) break;


            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::ostringstream oss;
            oss << "../saved_data/photo_" << t << ".jpg";
            lastPhoto = oss.str();



            std::ofstream file(lastPhoto, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Failed to open file for writing: " << lastPhoto << std::endl;
                break;
            }
            file.write(reinterpret_cast<const char*>(payload.data()), payload.size());
            file.close();

            std::cout << "Photo saved to: " << lastPhoto << std::endl;
            std::tm* tmInfo = std::localtime(&t);
            std::ostringstream timeOss;
            timeOss << std::put_time(tmInfo, "%b %d, %Y  %I:%M %p");
            storage.addImage(lastPhoto,timeOss.str());

            if (photoCallback)
                photoCallback(lastPhoto);
            break;
        }

        default:
            std::cout << "Unhandled command: " << static_cast<int>(cmd) << std::endl;
            break;
    }
}

/**
 * @brief Registers a callback invoked whenever a new video frame is received.
 *
 * The callback is fired from handlePacket() each time a Frame packet arrives,
 * passing the raw JPEG-encoded frame bytes directly to the caller.
 *
 * @param callback A callable that accepts a const reference to the JPEG byte vector.
 */
void Camera::onFrame(std::function<void(const std::vector<uint8_t>&)> callback) {
    frameCallback = callback;
}
/**
 * @brief Registers a callback invoked whenever a photo is saved to disk.
 *
 * The callback is fired from handlePacket() after a TakePhoto response is
 * received and the image has been written to disk, passing the saved file path.
 *
 * @param callback A callable that accepts the saved photo file path as a string.
 */
void Camera::onPhoto(std::function<void(const std::string&)> callback) {
    photoCallback = callback;
}
/**
 * @brief Returns the most recently received video frame and clears the new-frame flag.
 *
 * Acquires frameMutex before reading, making it safe to call from a different
 * thread than the one receiving packets.
 *
 * @return A copy of the latest JPEG-encoded frame as a byte vector.
 *         Returns an empty vector if no frame has been received yet.
 */
std::vector<uint8_t> Camera::getLatestFrame() {
    std::lock_guard<std::mutex> lock(frameMutex);
    newFrameAvailable = false;
    return latestFrame;
}
/**
 * @brief Returns whether a new frame has arrived since the last call to getLatestFrame().
 *
 * Acquires frameMutex before reading the flag, making it safe to poll from
 * any thread.
 *
 * @return true if a new frame is available, false otherwise.
 */
bool Camera::hasNewFrame() const {
    std::lock_guard<std::mutex> lock(frameMutex);
    return newFrameAvailable;
}

/**
 * @brief Returns the file path of the most recently saved photo.
 *
 * @return The path string set by the last successful TakePhoto response,
 *         or an empty string if no photo has been received yet.
 */
std::string Camera::lastPhotoPath() const {
    return lastPhoto;
}
