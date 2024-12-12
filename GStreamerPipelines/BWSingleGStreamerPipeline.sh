#!/bin/bash

pkill raspivid
pkill gst-launch-1.0

export DISPLAY=:0

# Send to Linux Laptop
# Put Laptop IP address here
# raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.140.56 port=5000 &

# Use libcamera-vid --list-cameras to get camera-name parameter
gst-launch-1.0 libcamerasrc ! \
    video/x-raw,width=1280,height=720,framerate=30/1 ! \
    videoconvert ! \
    autovideosink

gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.56 port=5001 &

# Pipeline to recieve frames from laptop
# Place RPI IP address here
gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5002 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false




