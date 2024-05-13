cmake_minimum_required(VERSION 3.10)
project(testProgram)

set(CMAKE_CXX_STANDARD 14)

# Add the path to WiringPi library
set(WIRINGPI_PATH /usr/local/lib)

# Find WiringPi library
find_library(WIRINGPI_LIB wiringPi PATHS ${WIRINGPI_PATH})
find_package(OpenCV REQUIRED)

# Check if WiringPi library is found
if(NOT WIRINGPI_LIB)
    message(FATAL_ERROR "WiringPi library not found.")
endif()

# Set the source file
set(SOURCE_FILES main.cpp)

# Create the executable
add_executable(testProgram ${SOURCE_FILES} yen_threshold.cpp)

target_include_directories(testProgram PRIVATE 
    ${OpenCV_INCLUDE_DIRS} 
    /home/pi/Desktop/Onboard_VS_Streaming_RPI/testingFolder/
    /home/pi/Desktop/Onboard_VS_Streaming_RPI/testingFolder/include
    )

# Link the WiringPi library
target_link_libraries(testProgram ${WIRINGPI_LIB} ${OpenCV_LIBS})
