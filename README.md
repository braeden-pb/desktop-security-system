# Group 55 — Security System

A C++ desktop security system application built with wxWidgets and SFML. The system supports arming/disarming via PIN, motion-triggered alarms with audio playback, image and video capture storage, and an alert notification system.

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building](#building)
- [Running the Application](#running-the-application)
- [Running Tests](#running-tests)
- [Project Structure](#project-structure)
---
## Prerequisites

The following must be installed on your system **before** building. SFML and GoogleTest are fetched automatically by CMake.

### All Platforms
- [CMake](https://cmake.org/) 3.14 or newer
- [Git](https://git-scm.com/) (required for CMake FetchContent)
- A C++20 compatible compiler:
    - **Linux/macOS:** GCC 10+ or Clang 12+
    - **Windows:** MSVC 2019+ or MinGW-w64

### wxWidgets

wxWidgets must be installed manually as it is not auto-fetched.

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install libwxgtk3.2-dev
```

**macOS:**
```bash
brew install wxwidgets
```

**Windows:**
Download the installer from [wxwidgets.org](https://wxwidgets.org/downloads/) and follow the setup guide. Make sure to include the `core`, `base`, and `media` components.

---

## Building

```bash
# Clone the repository
git clone https://gitlab.sci.uwo.ca/courses/2026/01/COMPSCI3307/group55.git
cd group55

# Create a build directory
mkdir build && cd build

# Configure
cmake ..

### Build the Application Only
cmake --build . --target SecuritySystem
```

> On first run, CMake will automatically download **SFML 3.0.0** and **GoogleTest 1.14.0** via FetchContent. This requires an internet connection.

### Build Type (Optional)

```bash
# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

---

## Running the Application

From the `build/` directory:

```bash
./SecuritySystem
```

**Default PIN:** `1234`

---

## Building & Running Tests

Each test suite is a separate CMake target. Build and run them individually as needed.

### Alarm Tests
```bash
cmake --build . --target AlarmTest
./AlarmTest
```

### Storage Tests
```bash
cmake --build . --target StorageTests
./StorageTests
```

### Image Tests
```bash
cmake --build . --target ImageTests
./ImageTests
```

### File Tests
```bash
cmake --build . --target FileTests
./FileTests
```

### System Tests
```bash
cmake --build . --target SystemTests
./SystemTests
```

### Video Tests
```bash
cmake --build . --target VideoTests
./VideoTests
```

### Build and Run All Tests at Once

```bash
cmake --build . --target StorageTests AlarmTest ImageTests FileTests SystemTests VideoTests
ctest --output-on-failure
```


---

## Project Structure

```
group55/
├── main.cpp
├── CMakeLists.txt
│
├── SecuritySystem.cpp / .h     # Central coordinator
├── Alarm.cpp / .h              # SFML audio alarm
├── Alert.cpp / .h              # Alert and notification logic
├── UI.cpp / .h                 # wxWidgets UI shell
├── Home_Panel.cpp / .h         # Main dashboard panel
├── Login_Panel.cpp / .h        # PIN entry panel
├── Config_Panel.cpp / .h       # System configuration panel
├── Storage_Panel.cpp / .h      # Image/video browser panel
├── Storage.cpp / .h            # Persistence layer
├── Image.cpp / .h              # Image model
├── Video.cpp / .h              # Video model
├── File.cpp / .h               # Base file model
│
└── tests/
    ├── AlarmTest.cpp
    ├── StorageTests.cpp
    ├── ImageTests.cpp
    ├── FileTests.cpp
    ├── SystemTest.cpp
    └── VideoTests.cpp
```

---
