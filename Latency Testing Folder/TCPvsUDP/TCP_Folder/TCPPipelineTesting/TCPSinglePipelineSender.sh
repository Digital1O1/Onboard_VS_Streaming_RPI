#!/bin/bash

pkill raspivid
pkill gst-launch-1.0

export DISPLAY=:0

# Sender pipeline
# Use Laptop IP address at end

# Use this and don't specify host IP address since RPI is acting as a server and allowing any device to connnect to it
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! \
h264parse ! rtph264pay config-interval=-1 pt=96 ! rtpstreampay ! \
tcpserversink host=0.0.0.0 port=7001
