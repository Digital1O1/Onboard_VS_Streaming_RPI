#!/bin/bash


pkill gst-launch-1.0

# Basic pipeline structure
# gst-launch-1.0 <source> ! <filter> ! <sink>


# This works on the RPI side

gst-launch-1.0 libcamerasrc ! \
video/x-raw,width=640,height=480,framerate=30/1 ! \
jpegenc ! tcpserversink host=0.0.0.0 port=5000

