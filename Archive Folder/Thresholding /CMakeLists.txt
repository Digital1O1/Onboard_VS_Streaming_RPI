cmake_minimum_required(VERSION 3.10)
project(Thresholding)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 14)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add executable
add_executable(Thresholding main.cpp yen_threshold.cpp) # Replace main.cpp with your source file name

# Include directories
target_include_directories(Thresholding PRIVATE 
    ${OpenCV_INCLUDE_DIRS} 
    /home/pi/Desktop/Onboard_VS_Streaming_RPI/Thresholding/include)

# Linking
target_link_libraries(Thresholding ${OpenCV_LIBS})
