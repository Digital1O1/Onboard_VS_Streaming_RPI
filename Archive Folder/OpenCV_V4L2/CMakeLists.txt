cmake_minimum_required(VERSION 3.12)
project(OpenCV_V4L2_Example)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

add_executable(OpenCV_V4L2_Example main.cpp)

target_link_libraries(OpenCV_V4L2_Example ${OpenCV_LIBS})
