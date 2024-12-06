#!/bin/bash

pkill raspivid
pkill gst-launch-1.0

export DISPLAY=:0

# Send to Linux Laptop
# Put Laptop IP address here
gst-launch-1.0 libcamerasrc ! video/x-raw,width=640,height=480,framerate=30/1 ! videoconvert ! v4l2h264enc ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.140.56 port=5001 &

# Pipeline to recieve frames from laptop
# Place RPI IP address here
gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5002 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false




