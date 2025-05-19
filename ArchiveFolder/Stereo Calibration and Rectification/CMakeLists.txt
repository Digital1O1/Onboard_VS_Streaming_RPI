cmake_minimum_required(VERSION 3.10)
project(StereoCalibration)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 14)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(StereoCalibration main.cpp) # Replace main.cpp with your source file name
add_executable(TakePicture takePicture.cpp) # Replace main.cpp with your source file name

# Linking
target_link_libraries(StereoCalibration ${OpenCV_LIBS})
target_link_libraries(TakePicture ${OpenCV_LIBS})
