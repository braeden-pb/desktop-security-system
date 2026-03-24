cmake_minimum_required(VERSION 3.15)
project(group55)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# OpenCV
FetchContent_Declare(
        opencv
        GIT_REPOSITORY https://github.com/opencv/opencv.git
        GIT_TAG        4.9.0
)
FetchContent_MakeAvailable(opencv)

# WiringPi (no CMakeLists.txt, manual build)
FetchContent_Declare(
        wiringpi
        GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
        GIT_TAG        3.10
)
FetchContent_MakeAvailable(wiringpi)

add_library(wiringPi STATIC
        ${wiringpi_SOURCE_DIR}/wiringPi/wiringPi.c
        ${wiringpi_SOURCE_DIR}/wiringPi/wiringPiI2C.c
        ${wiringpi_SOURCE_DIR}/wiringPi/wiringPiSPI.c
        ${wiringpi_SOURCE_DIR}/wiringPi/wiringSerial.c
        ${wiringpi_SOURCE_DIR}/wiringPi/wiringShift.c
        ${wiringpi_SOURCE_DIR}/wiringPi/piHiPri.c
        ${wiringpi_SOURCE_DIR}/wiringPi/piThread.c
        ${wiringpi_SOURCE_DIR}/wiringPi/softPwm.c
        ${wiringpi_SOURCE_DIR}/wiringPi/softTone.c
)
target_include_directories(wiringPi PUBLIC ${wiringpi_SOURCE_DIR}/wiringPi)
target_link_libraries(wiringPi PUBLIC pthread crypt m)


#find_package(PkgConfig REQUIRED)
#pkg_check_modules(LIBCAMERA REQUIRED libcamera)

# Pi executable
add_executable(pi-camera
        main.cpp
        NetworkServer.cpp NetworkServer.h
        Motion_Sensor_PI.cpp Motion_Sensor_PI.h
)

# Include shared protocol
target_include_directories(pi-camera PRIVATE
        ${CMAKE_SOURCE_DIR}/../Shared
)

# Link libraries
target_link_libraries(pi-camera PRIVATE
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_videoio
        wiringPi
)

# Compiler warnings
target_compile_options(pi-camera PRIVATE -Wall -Wextra)