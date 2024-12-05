#!/bin/bash

# Send video stream with a resolution of 640x480
gst-launch-1.0 -v v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480 ! videoconvert ! x264enc tune=zerolatency bitrate=500 ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host=172.17.140.56 port=5000 & 

# Receive and display video stream
gst-launch-1.0 -v udpsrc port=5002 caps="application/x-rtp, encoding-name=H264, payload=96" ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink sync=false

