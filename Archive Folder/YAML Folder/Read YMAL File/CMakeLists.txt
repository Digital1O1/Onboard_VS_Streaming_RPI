cmake_minimum_required(VERSION 3.10.0)
project(ReadYAML)

# Set the C++ standard you are using (e.g., C++11, C++14, C++17)
set(CMAKE_CXX_STANDARD 11)

# Find OpenCV package in the system
find_package(OpenCV REQUIRED)

# Find yaml-cpp
find_package(yaml-cpp REQUIRED)
# Add your source file
set(SOURCES
    main.cpp
)

# Include directories for OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the executable for your application
#add_executable(camera_calibration camera_calibration.cpp)
add_executable(ReadYAML ${SOURCES})

# Link your application with OpenCV libraries
target_link_libraries(ReadYAML ${OpenCV_LIBS} yaml-cpp)
