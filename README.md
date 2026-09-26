# Security System

A C++ desktop security system application built with wxWidgets. The system connects to a Raspberry Pi over a local network to provide live camera streaming, motion detection, alarm triggering with audio playback, photo capture, and a storage browser.

---

## Table of Contents
- [Prerequisites](#prerequisites)
- [Building the Desktop App](#building-the-desktop-app)
- [Building the Pi Server](#building-the-pi-server)
- [Running the Application](#running-the-application)
- [Running Tests](#running-tests)
- [Project Structure](#project-structure)
- [Hardware Setup](#hardware-setup)

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

# Create a build
