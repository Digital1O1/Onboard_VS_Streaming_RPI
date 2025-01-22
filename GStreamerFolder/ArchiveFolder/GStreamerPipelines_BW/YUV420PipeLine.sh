#!/bin/bash
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
    video/x-raw, format=I420, width=640, height=480,framerate=30/1 ! \
    videoconvert ! \
    autovideosink