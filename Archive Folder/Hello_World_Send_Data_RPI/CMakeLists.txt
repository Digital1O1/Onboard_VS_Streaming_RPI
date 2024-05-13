cmake_minimum_required(VERSION 3.0)
project(Hello_World_Send_Data_RPI)

# Add your source files
set(SOURCES
    main.cpp
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(Hello_World_Send_Data_RPI ${SOURCES})

# Find and link the required libraries (if any)
find_package(Threads REQUIRED)
target_link_libraries(Hello_World_Send_Data_RPI Threads::Threads)
