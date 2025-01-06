#!/bin/bash

export DISPLAY=:0

# Pipeline to send frames to laptop
pkill raspivid 
pkill gst-launch-1.0
export DISPLAY=:0
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.174 port=5000 &

./resize_window.sh &


#./resize_window

# Don't use outside of data gathering purposes 
# gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,width=640,height=480,framerate=60/1,format=NV12 ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.1,profile=(string)baseline' ! h264parse ! rtph264pay ! udpsink host=172.17.141.174 port=5000 &

# Pipeline to recieve frames from laptop
gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5003 caps=application/x-rtp ! \
rtph264depay ! h264parse  ! v4l2h264dec ! autovideosink sync=false

# Using videoscale and upscaling the resolution to 1080x720 increases latency significantly
#gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5003 caps=application/x-rtp ! \
#rtph264depay ! h264parse  ! v4l2h264dec ! videoscale ! video/x-raw,width=1080,height=720 ! autovideosink sync=false

# Clean up
#pkill resize_window
pkill raspivid 
pkill gst-launch-1.0