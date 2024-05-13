cmake_minimum_required(VERSION 3.10)
project(YAML_Example)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 14)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Find yaml-cpp
find_package(yaml-cpp REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(YAML_Example main.cpp) # Replace main.cpp with your source file name

# Linking
target_link_libraries(YAML_Example ${OpenCV_LIBS} yaml-cpp)
