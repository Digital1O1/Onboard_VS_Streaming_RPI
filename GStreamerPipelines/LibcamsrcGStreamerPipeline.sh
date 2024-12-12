#!/bin/bash


# Use libcamera-vid --list-cameras to get camera-name parameter
# Camera 0
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
    video/x-raw,width=640,height=480,framerate=30/1 ! \
    videoconvert ! \
    autovideosink

# Camera 1
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! \
    video/x-raw,width=640,height=480,framerate=30/1 ! \
    queue ! \
    videoconvert ! \
    autovideosink

# Pipelines currently being used in Mock CVG program

    # std::string visibleCameraPipeline = R"(
    #     libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! 
    #     video/x-raw,width=640,height=480,framerate=30/1 ! 
    #     videoconvert ! 
    #     queue ! 
    #     video/x-raw,format=(string)BGR ! 
    #     appsink
    # )";

    # std::string irCameraPipeline = R"(
    #     libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! 
    #     video/x-raw,width=640,height=480,framerate=30/1 ! 
    #     videoconvert ! 
    #     queue ! 
    #     video/x-raw,format=(string)BGR ! 
    #     appsink
    # )";