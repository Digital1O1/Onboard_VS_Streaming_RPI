cmake_minimum_required(VERSION 3.0)
project(dynamicPicture)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add your source file
set(SOURCES
    main.cpp
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(dynamicPicture ${SOURCES})

# Link OpenCV
target_link_libraries(dynamicPicture ${OpenCV_LIBS})
