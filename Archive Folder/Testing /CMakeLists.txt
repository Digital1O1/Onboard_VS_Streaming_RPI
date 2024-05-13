cmake_minimum_required(VERSION 3.10)
project(libcamera_example)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

add_executable(libcamera_example main.cpp)

target_include_directories(libcamera_example PRIVATE ${LIBCAMERA_INCLUDE_DIRS})
target_link_libraries(libcamera_example PRIVATE ${LIBCAMERA_LIBRARIES})
