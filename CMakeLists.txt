cmake_minimum_required(VERSION 3.13)

project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01d317aef9846059-450f68d30e039c26.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
        Storage.cpp
        Storage.h
        File.cpp
        File.h
        Image.cpp
        Image.h
        Video.cpp
        Video.h
)

target_link_libraries(SecuritySystem ${wxWidgets_LIBRARIES})

add_executable(ImageTests
        ImageTests.cpp
        Image.cpp
        Video.cpp
        File.cpp
)
target_link_libraries(ImageTests GTest::gtest_main ${wxWidgets_LIBRARIES})