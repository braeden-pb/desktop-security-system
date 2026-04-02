cmake_minimum_required(VERSION 3.15)
project(group55)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)


#WiringPi (no CMakeLists.txt, manual build)
#FetchContent_Declare(
#        wiringpi
#        GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
#        GIT_TAG        3.10
#)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
        Alarm_PI.cpp Alarm_PI.h
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
        tests/Camera_test.cpp
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

add_executable(AlarmPI_Tests
        tests/AlarmPI_Tests.cpp
        Alarm_PI.cpp
        NetworkServer.cpp
        ../Shared/Protocol.h
)
target_include_directories(AlarmPI_Tests PRIVATE
        ${CMAKE_SOURCE_DIR}/../Shared
)
target_link_libraries(AlarmPI_Tests PRIVATE GTest::gtest_main)

add_executable(CameraPI_Tests
        tests/CameraPI_Tests.cpp
        Camera_PI.cpp
        NetworkServer.cpp
        ../Shared/Protocol.h
)
target_include_directories(CameraPI_Tests PRIVATE
        ${CMAKE_SOURCE_DIR}/../Shared
        ${CMAKE_SOURCE_DIR}
        ${LIBCAMERA_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(CameraPI_Tests PRIVATE
        GTest::gtest_main
        camera
        camera-base
        GTest::gmock
        ${OpenCV_LIBS}
)

add_executable(MotionSensorPI_Tests
        tests/MotionSensorPI_Tests.cpp
        Motion_Sensor_PI.cpp
        ../Shared/Protocol.h
)
target_include_directories(MotionSensorPI_Tests PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/../Shared
)
target_link_libraries(MotionSensorPI_Tests PRIVATE
        GTest::gtest_main
        GTest::gmock
        wiringPi
)

add_executable(NetworkServer_Tests
        tests/NetworkServer_Tests.cpp
        NetworkServer.cpp
        ../Shared/Protocol.h
)
target_include_directories(NetworkServer_Tests PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/../Shared
)
target_link_libraries(NetworkServer_Tests PRIVATE GTest::gtest_main)

enable_testing()
add_test(NAME AlarmPITests COMMAND AlarmPI_Tests)
add_test(NAME CameraPITests COMMAND CameraPI_Tests)
add_test(NAME MotionSensorPITests COMMAND MotionSensorPI_Tests)
add_test(NAME NetworkServerTests COMMAND NetworkServer_Tests)