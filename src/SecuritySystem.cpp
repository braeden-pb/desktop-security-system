#include "SecuritySystem.h"

#include "Alarm.h"
#include "Config.h"
#include "UI.h"
#include "Storage.h"
#include "Alert.h"
#include "Camera.h"
#include "Motion_Sensor.h"
#include "Network.h"


/**
 * @brief Constructs a fully initialized SecuritySystem with UI, storage, and alarm.
 *
 * Creates all subsystems — @ref Storage, @ref UI, and @ref Alarm — and sets
 * the initial system status to disarmed.
 */
SecuritySystem::SecuritySystem() {
    mainStorage = std::make_unique<Storage>("../saved_data/");
    network = std::make_unique<Network>();
    if (network->connect("192.168.2.174", 5000)) {
        std::cout << "Connected to Pi" << std::endl;
    } else {
        std::cerr << "Could not connect to Pi" << std::endl;
    }
    network->startReceiving();
    camera = std::make_unique<Camera>(*network);
    mainUi = std::make_unique<UI>(this);
    systemStatus = Status::disarmed;
    alarm = std::make_unique<Alarm>();
    config = std::make_unique<Config>();
    motion_sensor = std::make_unique<Motion_Sensor>();
    addObserver(mainUi.get());
    addObserver(alarm.get());
    addObserver(this);




}

/**
 * @brief Constructs a SecuritySystem with optional headless mode.
 *
 * In headless mode, no UI is created. Storage is always initialized.
 * The system starts in a disarmed state.
 *
 * @param headless If true, skips UI initialization for non-interactive use.
 *
 * @note The current implementation initializes Storage regardless of the
 *       headless flag. The conditional re-assignment appears to be a bug.
 */
SecuritySystem::SecuritySystem(bool headless) {
    mainStorage = std::make_unique<Storage>("../saved_data/");
    if (!headless) mainStorage = std::make_unique<Storage>("../saved_data/");
    systemStatus = Status::disarmed;
    alarm = std::make_unique<Alarm>();
    addObserver(alarm.get());
}

/**
 * @brief Destroys the SecuritySystem and releases all owned subsystems.
 */
SecuritySystem::~SecuritySystem() {}

/**
 * @brief Returns a non-owning pointer to the UI subsystem.
 *
 * @return UI* Pointer to the active UI instance, or nullptr if running headless.
 */
UI *SecuritySystem::getUI() const{
    return mainUi.get();
}

Config *SecuritySystem::getConfig() {
    return config.get();
}

Camera *SecuritySystem::getCamera() const {
    return camera.get();
}

bool SecuritySystem::isConnected() const {
    return network && network->isConnected();
}

void SecuritySystem::update(const std::string& event) {
    if (event == "Motion detected")
        sendCommand(System::Motion, Command::MotionDetected);
}

/**
 * @brief Sets the current armed/disarmed status of the system.
 *
 * @param status The new @ref Status value to apply.
 */
void SecuritySystem::setStatus(Status status) {
    systemStatus = status;
}

/**
 * @brief Activates the alarm.
 *
 * Delegates to @ref Alarm::activate(). The alarm will sound continuously
 * until @ref turnOffAlarm() is called.
 */
void SecuritySystem::soundAlarm() {
    notifyObservers("Alarm triggered");
}

/**
 * @brief Deactivates the alarm.
 *
 * Delegates to @ref Alarm::deactivate() to stop audio playback.
 *
 * @pre @ref soundAlarm() must have been called prior to this.
 */
void SecuritySystem::turnOffAlarm() const {
    alarm->deactivate();
}


/**
 * @brief Checks whether the alarm is currently sounding.
 *
 * @return true  if the alarm is active.
 * @return false if the alarm is inactive.
 */
bool SecuritySystem::getIsAlarmActive() const {
    return alarm->getStatus();
}

/**
 * @brief Validates a given PIN against the system's stored PIN.
 *
 * @param pin The PIN string to validate.
 * @return true  if the PIN matches.
 * @return false if the PIN is incorrect.
 *
 * @warning The PIN is currently hardcoded as "1234". This should be
 *          replaced with a secure lookup from @ref Storage before deployment.
 */
bool SecuritySystem::validatePIN(const std::string& pin) const {
    return pin == config->getPassword();
}

/**
 * @brief Returns whether the system is currently armed.
 *
 * @return true  if the system status is @ref Status::armed.
 * @return false otherwise.
 */
bool SecuritySystem::isArmed() const {
    return systemStatus == Status::armed;
}

/**
 * @brief Arms the security system.
 *
 * Sets the system status to @ref Status::armed. Once armed, any
 * triggered sensor should result in @ref soundAlarm() being called.
 */
void SecuritySystem::arm() {
    setStatus(Status::armed);
    activateHardware();
}

void SecuritySystem::activateHardware() {
    motion_sensor->addObserver(this);
    motion_sensor->activate();
}

/**
 * @brief Disarms the security system.
 *
 * Sets the system status to @ref Status::disarmed, preventing
 * further alarm triggers until re-armed.
 */
void SecuritySystem::disarm() {
    setStatus(Status::disarmed);
}

void SecuritySystem::sendCommand(System sys, Command cmd) {
    if (!network->isConnected()) return;

    PacketHeader header;
    header.system      = sys;
    header.command     = cmd;
    header.payloadSize = 0;
    network->send(header, {});
}

/**
 * @brief Returns a non-owning pointer to the Storage subsystem.
 *
 * @return Storage* Pointer to the active Storage instance.
 */
Storage *SecuritySystem::getStorage() const {
    return mainStorage.get();
}

/**
 * @brief Retrieves all stored images as a flat list of tuples.
 *
 * Queries @ref Storage for all image records and unpacks each into
 * a tuple of (ID, timestamp, file path).
 *
 * @return A list of tuples where each entry contains:
 *         - `int`         — the image ID
 *         - `std::string` — the timestamp the image was captured
 *         - `std::string` — the file path to the image on disk
 */
std::list<std::tuple<int,std::string,std::string>> SecuritySystem::getAllImages() {
    std::list<std::tuple<int,std::string,std::string>> paths;
    // Get the list of objects from your storage class
    auto images = mainStorage->listImage();

        for (const auto& img : images) {
            paths.emplace_back(img.getID(), img.getTimeStamp(), img.getPath());
        }
    return paths;
}

void SecuritySystem::addObserver(Observer* o) {
    observers.push_back(o);
}

void SecuritySystem::notifyObservers(const std::string& event) {
    for (auto* o : observers) {
        o->update(event);
    }
}

void SecuritySystem::soundAlert() {
    notifyObservers("Motion detected");
}

void SecuritySystem::setUI(UI* ui) {
    m_ui = ui;
}

void SecuritySystem::triggerAlert(const std::string& type) {
    if (m_ui) {
        m_ui->showAlert("Alert: " + type);
    }
}




