cmake_minimum_required(VERSION 3.14)
project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base media)
include(FetchContent)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)


FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
)
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(wxWidgets REQUIRED COMPONENTS core base media)
include(${wxWidgets_USE_FILE})


add_executable(SecuritySystem
        main.cpp
        SecuritySystem.cpp SecuritySystem.h
        UI.cpp UI.h
        Home_Panel.cpp Home_Panel.h
        Login_Panel.h Login_Panel.cpp
        Config_Panel.cpp Config_Panel.h
        Storage_Panel.cpp Storage_Panel.h
        Storage.cpp Storage.h
        Image.cpp Image.h
        File.cpp File.h
        Alarm.cpp Alarm.h
        Config.cpp
        Device.cpp Device.h
        Network.cpp Network.h
        ../Shared/Protocol.h
        Motion_Sensor.cpp Motion_Sensor.h

)

add_executable(StorageTests
        tests/StorageTests.cpp
        Storage.cpp
        Image.cpp
        File.cpp
)

target_link_libraries(StorageTests GTest::gtest_main ${wxWidgets_LIBRARIES})

add_executable(ImageTests
        tests/ImageTests.cpp
        Image.cpp
        File.cpp
)
add_executable(FileTests
        tests/FileTests.cpp
        File.cpp
)
add_executable(SystemTests
        tests/SystemTest.cpp
        SecuritySystem.cpp
        Alarm.cpp
        Storage.cpp
        File.cpp
        UI.cpp
        Image.cpp
        Home_Panel.cpp
        Login_Panel.cpp
        Storage_Panel.cpp

)

add_executable(AlertTest
        tests/AlertTest.cpp
        Alert.cpp
        Event.cpp
        SecuritySystem.cpp
)

target_link_libraries(AlertTest ${wxWidgets_LIBRARIES})

target_link_libraries(SystemTests
        GTest::gtest_main
        sfml-audio
        sfml-system
        ${wxWidgets_LIBRARIES}
)

target_link_libraries(FileTests GTest::gtest_main ${wxWidgets_LIBRARIES})

target_link_libraries(ImageTests GTest::gtest_main ${wxWidgets_LIBRARIES})

add_executable(VideoTests
        tests/VideoTests.cpp
        Video.cpp
        File.cpp
)
target_link_libraries(VideoTests GTest::gtest_main ${wxWidgets_LIBRARIES})

target_link_libraries(SecuritySystem PRIVATE ${wxWidgets_LIBRARIES} sfml-audio)

# Alarm User Story Acceptance Test
add_executable(AlarmTest tests/AlarmTest.cpp Alarm.cpp Alarm.h
        Observer.h
        Motion_Sensor.cpp
        Motion_Sensor.h
        Camera.cpp
        Camera.h)
target_link_libraries(AlarmTest PRIVATE GTest::gtest_main sfml-audio ${wxWidgets_LIBRARIES})

add_executable(ConfigTests tests/ConfigTests.cpp Config.cpp)

add_executable(network_test_client
        network_test_client.cpp
        Network.cpp Network.h
        ../Shared/Protocol.h
)

target_link_libraries(network_test_client PRIVATE ${wxWidgets_LIBRARIES})

add_executable(Camera-Test
        camera_test_PC.cpp
        Camera.cpp Camera.h
        Network.cpp Network.h
        ../Shared/Protocol.h
)

find_package(OpenCV REQUIRED)
target_link_libraries(Camera-Test PRIVATE ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})



enable_testing()
add_test(NAME AlarmTests COMMAND AlarmTest)
add_test(NAME StorageTests COMMAND StorageTests)
add_test(NAME VideoTests COMMAND VideoTests)
add_test(NAME SystemTests COMMAND SystemTests)
add_test(NAME FileTests COMMAND FileTests)
add_test(NAME ImageTests COMMAND ImageTests)
add_test(NAME ConfigTests COMMAND ConfigTests)