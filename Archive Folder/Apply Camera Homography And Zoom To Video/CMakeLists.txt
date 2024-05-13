cmake_minimum_required(VERSION 3.10)
project(ApplyHomography)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES
    main.cpp 
    yen_threshold.cpp
)
# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(ApplyHomography main.cpp) # Replace main.cpp with your source file name

# Include directories
target_include_directories(ApplyHomography PRIVATE 
    ${OpenCV_INCLUDE_DIRS} 
    /home/pi/Desktop/Onboard_VS_Streaming_RPI/Apply Camera Homography And Zoom To Video)

# Linking
target_link_libraries(ApplyHomography ${OpenCV_LIBS})
