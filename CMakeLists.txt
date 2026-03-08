cmake_minimum_required(VERSION 3.13)
project(group55)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Use a stable release of GoogleTest for better reliability in WSL
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0  # Stable version
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(wxWidgets REQUIRED COMPONENTS core base media) # Added 'media' here
include(${wxWidgets_USE_FILE})

# 2. The main App (Removed StorageTests.cpp from here)
add_executable(SecuritySystem
        main.cpp
        SecuritySystem.cpp SecuritySystem.h
        UI.cpp UI.h
        Home_Panel.cpp Home_Panel.h
        Storage.cpp Storage.h
        File.cpp File.h
        Image.cpp Image.h
        Video.cpp Video.h
)
target_link_libraries(SecuritySystem ${wxWidgets_LIBRARIES})

# 3. Dedicated Storage Tests
add_executable(StorageTests
        StorageTests.cpp
        Storage.cpp
        Image.cpp
        File.cpp
)
# Link GTest and wxWidgets (in case your headers need them)
target_link_libraries(StorageTests GTest::gtest_main ${wxWidgets_LIBRARIES})

# 4. Image Specific Tests
add_executable(ImageTests
        ImageTests.cpp
        Image.cpp
        File.cpp
)
target_link_libraries(ImageTests GTest::gtest_main ${wxWidgets_LIBRARIES})

# 5. Video Specific Tests
add_executable(VideoTests
        VideoTests.cpp
        Video.cpp
        File.cpp
)
target_link_libraries(VideoTests GTest::gtest_main ${wxWidgets_LIBRARIES})

# 6. File Specific Tests
add_executable(FileTests
        FileTests.cpp
        File.cpp
)
target_link_libraries(FileTests GTest::gtest_main)