cmake_minimum_required(VERSION 3.15)
project(group55)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)


# WiringPi (no CMakeLists.txt, manual build)
#FetchContent_Declare(
#        wiringpi
#        GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
#        GIT_TAG        3.10
#)
find_library(WIRINGPI_LIB wiringPi REQUIRED)
set(GPIO_LIB ${WIRINGPI_LIB})


find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)
link_directories(${LIBCAMERA_LIBRARY_DIRS} ${OpenCV_LIBS})

# Pi executable
add_executable(pi-camera
        main.cpp
        NetworkServer.cpp NetworkServer.h
        Motion_Sensor_PI.cpp Motion_Sensor_PI.h
        Camera_PI.cpp
        Camera_PI.h
)

# Include shared protocol
target_include_directories(pi-camera PRIVATE
        ${CMAKE_SOURCE_DIR}/../Shared
        ${LIBCAMERA_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(pi-camera PRIVATE
        #opencv_core
        #opencv_imgproc
        #opencv_highgui
        #opencv_videoio
        wiringPi
        camera
        camera-base
        ${OpenCV_LIBS}
)

# Compiler warnings
target_compile_options(pi-camera PRIVATE -Wall -Wextra)

add_executable(camera-test
        Camera_test.cpp
        NetworkServer.cpp NetworkServer.h
        Camera_PI.cpp Camera_PI.h
)

target_include_directories(camera-test PRIVATE
        ${CMAKE_SOURCE_DIR}/../Shared
        ${LIBCAMERA_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(camera-test PRIVATE
        camera
        camera-base
        ${OpenCV_LIBS}
)

target_compile_options(camera-test PRIVATE -Wall -Wextra)
