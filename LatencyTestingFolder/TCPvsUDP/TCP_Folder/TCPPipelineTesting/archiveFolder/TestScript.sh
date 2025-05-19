#!/bin/bash

pkill raspivid
pkill gst-launch-1.0

export DISPLAY=:0

# RPI Sender --> Laptop
# Use this and don't specify host IP address since RPI is acting as a server and allowing any device to connnect to it
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! \
h264parse ! rtph264pay config-interval=-1 pt=96 ! rtpstreampay ! \
tcpserversink host=0.0.0.0 port=7001 &

sleep 3

# Laptop --> RPI Recieve and display
gst-launch-1.0 -vvvv tcpclientsrc host=172.17.140.56 port=5003 !\
application/x-rtp-stream,encoding-name=H264 ! rtpstreamdepay ! \
rtph264depay ! h264parse ! avdec_h264 ! autovideosink sync=false
