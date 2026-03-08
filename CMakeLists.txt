cmake_minimum_required(VERSION 3.13)

project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

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
        tests/TEST_UI.cpp
)

target_link_libraries(SecuritySystem ${wxWidgets_LIBRARIES})