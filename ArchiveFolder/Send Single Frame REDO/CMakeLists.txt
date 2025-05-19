cmake_minimum_required(VERSION 3.0)
project(singleFrame)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add your source file
set(SOURCES
    singleFrame.cpp
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(singleFrame ${SOURCES})

# Link OpenCV
target_link_libraries(singleFrame ${OpenCV_LIBS})
