cmake_minimum_required(VERSION 3.0)
project(dynamicPictureRPI)

set(WIRINGPI_PATH /usr/local/lib)


# Find OpenCV package
find_library(WIRINGPI_LIB wiringPi PATHS ${WIRINGPI_PATH})
find_package(OpenCV REQUIRED)

# Add your source file
set(SOURCES
    main.cpp
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(dynamicPictureRPI ${SOURCES}yen_threshold.cpp)

# Link OpenCV
target_link_libraries(dynamicPictureRPI ${OpenCV_LIBS})
