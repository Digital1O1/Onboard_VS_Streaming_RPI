cmake_minimum_required(VERSION 3.10)
project(OpenCVHomographyExample)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 14)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(OpenCVHomographyExample main.cpp) # Replace main.cpp with your source file name

# Linking
target_link_libraries(OpenCVHomographyExample ${OpenCV_LIBS})
