cmake_minimum_required(VERSION 3.14) # FetchContent works better with 3.14+
project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base media)
include(FetchContent)

# 1. Fetch SFML for the "Alarm Noise" User Story (3 points)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
)
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

# 2. Fetch GoogleTest for your Stage #3 Acceptance Tests
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 3. Find wxWidgets (including 'media' for your Video story)
find_package(wxWidgets REQUIRED COMPONENTS core base media)
include(${wxWidgets_USE_FILE})

add_executable(SecuritySystem
        main.cpp
        SecuritySystem.cpp
        SecuritySystem.h
        UI.cpp
        UI.h
        Home_Panel.cpp
        Home_Panel.h
        Login_Panel.h Login_Panel.cpp
        Config_Panel.cpp
        Config_Panel.h
        Storage_Panel.cpp
        Storage_Panel.h
        Storage.cpp Storage.h
        Image.h Image.cpp
        File.cpp File.h
        Alarm.cpp Alarm.h

)

add_executable(StorageTests
        StorageTests.cpp
        Storage.cpp
        Image.cpp
        File.cpp
)
# Link GTest and wxWidgets (in case your headers need them)
target_link_libraries(StorageTests GTest::gtest_main ${wxWidgets_LIBRARIES})

add_executable(ImageTests
        ImageTests.cpp
        Image.cpp
        File.cpp
)
add_executable(FileTests
        FileTests.cpp
        File.cpp
)
target_link_libraries(FileTests GTest::gtest_main ${wxWidgets_LIBRARIES})

target_link_libraries(ImageTests GTest::gtest_main ${wxWidgets_LIBRARIES})

# 5. Video Specific Tests
add_executable(VideoTests
        VideoTests.cpp
        Video.cpp
        File.cpp
)
target_link_libraries(VideoTests GTest::gtest_main ${wxWidgets_LIBRARIES})

# Link both wxWidgets and the fetched SFML audio
target_link_libraries(SecuritySystem PRIVATE ${wxWidgets_LIBRARIES} sfml-audio)

# Alarm User Story Acceptance Test
add_executable(AlarmTest AlarmTest.cpp Alarm.cpp Alarm.h)
target_link_libraries(AlarmTest PRIVATE GTest::gtest_main sfml-audio ${wxWidgets_LIBRARIES})

enable_testing()
add_test(NAME AlarmTests COMMAND AlarmTest)