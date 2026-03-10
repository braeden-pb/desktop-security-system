cmake_minimum_required(VERSION 3.14) # FetchContent works better with 3.14+
project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Main Security System App
add_executable(SecuritySystem
        main.cpp SecuritySystem.cpp SecuritySystem.h
        UI.cpp UI.h Home_Panel.cpp Home_Panel.h
        Login_Panel.cpp Login_Panel.h Alarm.cpp Alarm.h
)
# Link both wxWidgets and the fetched SFML audio
target_link_libraries(SecuritySystem PRIVATE ${wxWidgets_LIBRARIES} sfml-audio)

# Alarm User Story Acceptance Test
add_executable(AlarmTest AlarmTest.cpp Alarm.cpp Alarm.h)
target_link_libraries(AlarmTest PRIVATE GTest::gtest_main sfml-audio ${wxWidgets_LIBRARIES})

enable_testing()
add_test(NAME AlarmTests COMMAND AlarmTest)