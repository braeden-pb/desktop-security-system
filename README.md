# Group 55 — Security System

A C++ desktop security system application built with wxWidgets. The system connects to a Raspberry Pi over a local network to provide live camera streaming, motion detection, alarm triggering with audio playback, photo capture, and a storage browser.

---

## Table of Contents
- [Prerequisites](#prerequisites)
- [Building the Desktop App](#building-the-desktop-app)
- [Building the Pi Server](#building-the-pi-server)
- [Running the Application](#running-the-application)
- [Running Tests](#running-tests)
- [Project Structure](#project-structure)

---

## Prerequisites

The following must be installed before building. GoogleTest is fetched automatically by CMake.

### Desktop App (Linux)

- [CMake](https://cmake.org/) 3.14 or newer
- [Git](https://git-scm.com/)
- A C++20 compatible compiler (GCC 10+ or Clang 12+)
- OpenCV 4.x
- wxWidgets 3.2+

**Install dependencies (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install libwxgtk3.2-dev libopencv-dev cmake build-essential git
```

### Raspberry Pi Server

- Raspberry Pi 4 or 5 running Raspberry Pi OS
- libcamera
- OpenCV 4.x
- WiringPi
- A compatible camera module (CSI)
- A MAX98357 I2S amplifier connected via GPIO pins

**Install dependencies on Pi:**
```bash
sudo apt update
sudo apt install libcamera-dev libopencv-dev cmake build-essential
# WiringPi
wget https://github.com/WiringPi/WiringPi/releases/download/3.2/wiringpi_3.2_arm64.deb
sudo dpkg -i wiringpi_3.2_arm64.deb
```

---

## Building the Desktop App
```bash
# Clone the repository
git clone https://gitlab.sci.uwo.ca/courses/2026/01/COMPSCI3307/group55.git
cd group55/src

# Create a build directory
mkdir cmake-build-debug && cd cmake-build-debug

# Configure and build
cmake ..
cmake --build . --target SecuritySystem
```

> On first run, CMake will automatically download **GoogleTest 1.14.0** via FetchContent. This requires an internet connection.

### Build Type (Optional)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake .. -DCMAKE_BUILD_TYPE=Release
```

---

## Building the Pi Server
```bash
cd group55/PI

mkdir build && cd build
cmake ..
cmake --build . --target pi-camera
```

---

## Running the Application

### Desktop
Make sure the Raspberry Pi server is running first, then from the build directory:
```bash
./SecuritySystem
```

**Default PIN:** `123`

### Raspberry Pi Server
```bash
./pi-camera
```
The Pi will print its IP address and begin listening on port `5000`. Make sure the desktop app is configured with the correct Pi IP address.

---

## Running Tests

Each test suite is a separate CMake target. Build and run them individually as needed.
```bash
# Alarm Tests
cmake --build . --target AlarmTest && ./AlarmTest

# Storage Tests
cmake --build . --target StorageTests && ./StorageTests

# Image Tests
cmake --build . --target ImageTests && ./ImageTests

# File Tests
cmake --build . --target FileTests && ./FileTests

# System Tests
cmake --build . --target SystemTests && ./SystemTests

# Build and run all tests at once
cmake --build . --target StorageTests AlarmTest ImageTests FileTests SystemTests
ctest --output-on-failure
```

---

## Project Structure
```
group55/
├── src/                            # Desktop application
│   ├── main.cpp
│   ├── CMakeLists.txt
│   ├── saved_data/                 # Stored photos
│   │
│   ├── SecuritySystem.cpp / .h    # Central coordinator
│   ├── Alarm.cpp / .h             # Alarm — sends audio command to Pi
│   ├── Alert.cpp / .h             # Alert and notification logic
│   ├── Camera.cpp / .h            # Camera — receives frames and photos from Pi
│   ├── Network.cpp / .h           # TCP client — connects to Pi
│   ├── Config.cpp / .h            # Persisted user configuration
│   ├── Storage.cpp / .h           # Photo storage and management
│   ├── Image.cpp / .h             # Image model
│   │
│   ├── UI.cpp / .h                # wxWidgets UI shell and observer
│   ├── Home_Panel.cpp / .h        # Main dashboard — stream, arm/disarm
│   ├── Login_Panel.cpp / .h       # PIN entry panel
│   ├── Config_Panel.cpp / .h      # Settings panel
│   ├── Storage_Panel.cpp / .h     # Photo browser panel
│   │
│   └── tests/
│       ├── AlarmTest.cpp
│       ├── StorageTests.cpp
│       ├── ImageTests.cpp
│       ├── FileTests.cpp
│       └── SystemTest.cpp
│
└── PI/                             # Raspberry Pi server
    ├── main.cpp
    ├── CMakeLists.txt
    ├── sounds/                     # WAV alarm sound files
    │
    ├── Camera_PI.cpp / .h         # libcamera capture, streaming, photos
    ├── Motion_Sensor_PI.cpp / .h  # PIR sensor via WiringPi ISR
    ├── Alarm_PI.cpp / .h          # Audio playback via SoX
    └── NetworkServer.cpp / .h     # TCP server — accepts desktop connection
```

---

## Hardware Setup

### Required Components
- Raspberry Pi 4 or 5
- Raspberry Pi Camera Module (CSI connector)
- PIR Motion Sensor connected to GPIO pin 17
- MAX98357 I2S Amplifier connected via I2S pins:
  - DIN → GPIO 21
  - BCLK → GPIO 18
  - LRCLK → GPIO 19
- Speaker connected to MAX98357 output

### Network
The desktop and Pi must be on the same network. The Pi's IP address is printed on startup. Update the IP in `SecuritySystem.cpp` if your network changes.
    ├── CMakeLists.txt
    ├── sounds/                     # WAV alarm sound files
    │
    ├── Camera_PI.cpp / .h         # libcamera capture, streaming, photos
    ├── Motion_Sensor_PI.cpp / .h  # PIR sensor via WiringPi ISR
    ├── Alarm_PI.cpp / .h          # Audio playback via SoX
    └── NetworkServer.cpp / .h     # TCP server — accepts desktop connection
```

---

## Hardware Setup

### Required Components
- Raspberry Pi 4 or 5
- Raspberry Pi Camera Module (CSI connector)
- PIR Motion Sensor connected to GPIO pin 17
- MAX98357 I2S Amplifier connected via I2S pins:
  - DIN → GPIO 21
  - BCLK → GPIO 18
  - LRCLK → GPIO 19
- Speaker connected to MAX98357 output

### Network
The desktop and Pi must be on the same network. The Pi's IP address is printed on startup. Update the IP in `SecuritySystem.cpp` if your network changes.