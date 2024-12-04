#!/bin/bash

pkill raspivid
pkill gst-launch-1.0
export DISPLAY=:0

# Send to Linux Laptop
# Put Laptop IP address here

# irCamera
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 0 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.30 port=5000 &

# visibleCamera
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.30 port=5001 &

# Place RPI IP address here
# Command to receive the stream
# Streaming PI : 172.17.141.124
# irCameraPipeline
gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5002 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false &

# visibleCameraPipeline
gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5003 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false
