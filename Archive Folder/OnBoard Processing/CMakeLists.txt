cmake_minimum_required(VERSION 3.0)
project(OnBoardProcessing)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add your source file
set(SOURCES
    main.cpp
    yen_thresholding.cpp
)

include_directories("/home/pi/Desktop/Coding_Folder/Onboard_VS_Streaming_RPI/OnBoard Processing/")

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Add the executable
add_executable(OnBoardProcessing ${SOURCES})

# Link OpenCV
target_link_libraries(OnBoardProcessing ${OpenCV_LIBS})
