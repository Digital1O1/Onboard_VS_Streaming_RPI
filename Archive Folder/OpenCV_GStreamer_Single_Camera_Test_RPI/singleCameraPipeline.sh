#!/bin/bash

# Command to start streaming from raspivid ORIGINAL
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.174 port=5000 &


# Things to try out with V4L2
# Look intro extra-controls
# Mess with : 'video/x-h264,level=(string)4.2,profile=(string)baseline'
# With this :  video/x-h264,level=(string)4.2,profile=(string)baseline-constrained

# gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,width=640,height=480,framerate=30/1,format=NV12 ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.1,profile=(string)baseline' ! h264parse ! rtph264pay ! udpsink host=172.17.141.174 port=5000 &




# gst-launch-1.0 v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480,framerate=30/1,format=NV12 ! v4l2h264enc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000 &

# Command to receive the stream from Linux laptop --> back to RPI
# address == RPI IP address
# ORIGINAL
#gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5001 caps=application/x-rtp ! \
#rtph264depay ! h264parse  ! v4l2h264dec output-io-mode=4 capture-io-mode=4 ! autovideosink sync=false

gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5002 caps=application/x-rtp ! \
rtph264depay ! h264parse  ! v4l2h264dec output-io-mode=4 capture-io-mode=4 ! autovideosink sync=false



# Laptop check encoders
# RPI side check decoder