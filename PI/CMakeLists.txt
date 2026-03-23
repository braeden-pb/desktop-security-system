make_minimum_required(VERSION 3.15)
project(PiCamera)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies


# Pi executable
add_executable(pi-camera
        main.cpp
        NetworkServer.cpp NetworkServer.h
)

# Include shared protocol
target_include_directories(pi-camera PRIVATE
        ${CMAKE_SOURCE_DIR}/../Shared     # Protocol.h
        ${OpenCV_INCLUDE_DIRS}
        ${LIBCAMERA_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(pi-camera PRIVATE
        ${OpenCV_LIBS}
        ${LIBCAMERA_LIBRARIES}
)

# Compiler warnings
target_compile_options(pi-camera PRIVATE -Wall -Wextra)