cmake_minimum_required(VERSION 3.13)

project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0  # Stable version
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
target_link_libraries(FileTests GTest::gtest_main)

target_link_libraries(ImageTests GTest::gtest_main ${wxWidgets_LIBRARIES})

# 5. Video Specific Tests
add_executable(VideoTests
        VideoTests.cpp
        Video.cpp
        File.cpp
)
target_link_libraries(VideoTests GTest::gtest_main ${wxWidgets_LIBRARIES})

target_link_libraries(SecuritySystem ${wxWidgets_LIBRARIES})