cmake_minimum_required(VERSION 3.0)
project(FeatureMatching)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Define the executable
add_executable(FeatureMatching main.cpp)

# Link libraries
target_link_libraries(FeatureMatching ${OpenCV_LIBS})
