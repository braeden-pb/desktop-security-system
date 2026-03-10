cmake_minimum_required(VERSION 3.13)

project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

find_package(GTest REQUIRED)
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

add_executable(SecuritySystem
        main.cpp
        SecuritySystem.cpp
        SecuritySystem.h
        UI.cpp
        UI.h
        Home_Panel.cpp
        Home_Panel.h
        Login_Panel.cpp
        Login_Panel.h
        Alarm.cpp
        Alarm.h
)

target_link_libraries(SecuritySystem
        ${wxWidgets_LIBRARIES}
        sfml-audio
)

add_executable(AlarmTest
        AlarmTest.cpp
        Alarm.cpp
        Alarm.h
)

target_link_libraries(AlarmTest
        GTest::gtest
        GTest::gtest_main
        ${wxWidgets_LIBRARIES}
        sfml-audio
        pthread
)

enable_testing()
add_test(NAME AlarmTests COMMAND AlarmTest)