/**
 * @file Motion_Sensor_PI.cpp
 * @brief Implementation of the Motion_Sensor_PI class for PIR motion detection on Raspberry Pi.
 * @author evan, Rushd
 * @date 2026-03-24
 */

#include "Motion_Sensor_PI.h"
#include "NetworkServer.h"

/**
 * @brief Singleton instance pointer used by the static ISR handler to access the live object.
 *
 * WiringPi ISR callbacks must be free or static functions, so this pointer
 * bridges the gap between the static isrHandler() and the instance's state.
 */

Motion_Sensor_PI* Motion_Sensor_PI::instance_ = nullptr;

/**
 * @brief GPIO pin number (BCM numbering) connected to the PIR sensor's output.
 */

const int PIR_PIN = 17;


/**
 * @brief Constructs a Motion_Sensor_PI object and initializes the GPIO hardware.
 *
 * Stores the singleton instance pointer, initializes WiringPi in BCM GPIO mode,
 * and configures PIR_PIN as an input. The sensor is not activated at construction;
 * call activate() to begin detection.
 *
 * @param sensitivity  Sensitivity level for motion detection (reserved for future use).
 * @param motionSleep  Sleep interval in milliseconds between sensor polls (reserved for future use).
 * @param network      Reference to the NetworkServer used to send motion event packets.
 *
 * @throws std::runtime_error If WiringPi GPIO initialization fails.
 */

Motion_Sensor_PI::Motion_Sensor_PI(int sensitivity, int motionSleep, NetworkServer& network)
    : active(false), motionDetected(false), sensitivity(sensitivity),
      motionSleep(motionSleep), network(network) {
    instance_ = this;
    if (wiringPiSetupGpio() == -1)
        throw std::runtime_error("Failed to initialize WiringPi");
    pinMode(PIR_PIN, INPUT);
    // removed thread from here
}
/**
 * @brief Activates the motion sensor and begins processing motion events.
 *
 * Sets the active flag to true, then spawns a background sensor thread that
 * polls the motionPending flag every 10 ms. When a pending motion event is
 * detected, the thread constructs a MotionDetected packet and sends it via
 * the NetworkServer. After starting the thread, detectMotion() is called to
 * register the GPIO ISR.
 *
 * @note This method returns immediately after starting the thread and registering
 *       the ISR; motion detection runs asynchronously in the background.
 */
void Motion_Sensor_PI::activate() {
        active = true;
    sensorThread = std::thread([this]() {
        while (active) {
            if (motionPending) {
                motionPending = false;
                PacketHeader header{};
                header.system      = System::Motion;
                header.command     = Command::MotionDetected;
                header.payloadSize = 0;
                network.sendPacket(header, {});
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
	    detectMotion();
    }

/**
 * @brief Returns whether the sensor is currently active.
 *
 * @return true if the sensor has been activated and the background thread is running,
 *         false otherwise.
 */

bool Motion_Sensor_PI::isActive() {
    return active;
}

/**
 * @brief Registers the GPIO interrupt service routine for the PIR sensor pin.
 *
 * Attaches isrHandler() to PIR_PIN on both rising and falling edges using
 * WiringPi's wiringPiISR(). From this point onward, any state change on the
 * PIR output triggers the ISR asynchronously.
 *
 * @note Called internally by activate(). Should not be called directly.
 */

void Motion_Sensor_PI::detectMotion() {
    std::cout << "detectMotion started\n";
        wiringPiISR(PIR_PIN, INT_EDGE_BOTH, &Motion_Sensor_PI::isrHandler);
}

/**
 * @brief Marks a motion event as detected and pending transmission.
 *
 * Sets both motionDetected and motionPending to true. The background sensor
 * thread picks up motionPending and handles the actual network transmission,
 * keeping this method safe to call from interrupt context.
 *
 * @note This method is not currently called by isrHandler(), which sets the
 *       flags directly. It is retained for potential future use.
 */

void Motion_Sensor_PI::onMotion() {
    motionDetected = true;
    motionPending = true;  // just set flag, thread does the send
}

/**
 * @brief Returns whether motion is currently detected by the PIR sensor.
 *
 * @return true if the PIR pin was last read HIGH, false if it was read LOW.
 */

bool Motion_Sensor_PI::isMotionDetected() {
    return motionDetected;
}

/**
 * @brief Static interrupt service routine triggered on PIR_PIN state changes.
 *
 * Called by WiringPi on both rising and falling edges of PIR_PIN. On a rising
 * edge (motion detected), enforces a 5-second cooldown between consecutive
 * MotionDetected packets by comparing the current time against lastSent.
 * On a falling edge (motion cleared), resets the motionDetected flag.
 *
 * Early-exits if no instance is available or if a motion event is already
 * pending transmission to avoid flooding the network.
 *
 * @note This is a static function required by the WiringPi ISR API.
 *       It accesses instance state through the instance_ singleton pointer.
 * @warning Must not block or perform heavy work — it executes in interrupt context.
 */

void Motion_Sensor_PI::isrHandler() {
    if (!instance_) return;
    if (instance_->motionPending) return;  // ADD THIS - already waiting to send

    bool detected = digitalRead(PIR_PIN) == HIGH;

    if (detected) {
        time_t now = time(nullptr);
        if (difftime(now, instance_->lastSent) >= 3.0) {
            instance_->lastSent = now;
            instance_->motionDetected = true;
            instance_->motionPending = true;  // set flag directly, skip onMotion()
        }
    } else {
        instance_->motionDetected = false;
    }
}


/**
 * @brief Deactivates the motion sensor and stops the background thread.
 *
 * Clears the active flag and blocks until the sensor thread has finished
 * its current iteration and joined. The GPIO ISR remains registered but
 * will have no effect once the thread is no longer running.
 */

void Motion_Sensor_PI::deactivate() {
    active = false;
    if (sensorThread.joinable())
        sensorThread.join();
}

/**
 * @brief Destructs the Motion_Sensor_PI object.
 *
 * No explicit cleanup is required beyond what deactivate() handles;
 * the destructor is defined to maintain clear ownership semantics.
 */

Motion_Sensor_PI::~Motion_Sensor_PI() {
    deactivate();
}
