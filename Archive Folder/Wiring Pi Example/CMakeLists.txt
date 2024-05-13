cmake_minimum_required(VERSION 3.10)
project(GpioControl)

# Add the path to WiringPi library
set(WIRINGPI_PATH /usr/local/lib)

# Find WiringPi library
find_library(WIRINGPI_LIB wiringPi PATHS ${WIRINGPI_PATH})

# Check if WiringPi library is found
if(NOT WIRINGPI_LIB)
    message(FATAL_ERROR "WiringPi library not found.")
endif()

# Set the source file
set(SOURCE_FILES main.cpp)

# Create the executable
add_executable(gpio_control ${SOURCE_FILES})

# Link the WiringPi library
target_link_libraries(gpio_control ${WIRINGPI_LIB})
