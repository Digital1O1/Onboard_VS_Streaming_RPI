#!/bin/bash

pkill raspivid
pkill gst-launch-1.0
export DISPLAY=:0

WIDTH=1280
HEIGHT=720
RPI_TO_LAPTOP_PORT_NIR=7001
RPI_TO_LAPTOP_PORT_VIS=7002
LAPTOP_IP=172.17.140.56
RPI_IP=172.17.141.11
PROCESSED_NIR_PORT=5003
PROCESSED_VIS_PORT=5004
SET_FPS=30

# irCamera use Laptop IP address here
raspivid -t 0 -n -w $WIDTH -h $HEIGHT -fps $SET_FPS -b 2000000 -o - -cs 0 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=$LAPTOP_IP port=$RPI_TO_LAPTOP_PORT_NIR &

# visibleCamera
raspivid -t 0 -n -w $WIDTH -h $HEIGHT -fps $SET_FPS -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=$LAPTOP_IP port=$RPI_TO_LAPTOP_PORT_VIS &

# irCameraPipeline
gst-launch-1.0 -v udpsrc address=$RPI_IP port=$PROCESSED_NIR_PORT caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false &

# visibleCameraPipeline
gst-launch-1.0 -v udpsrc address=$RPI_IP port=$PROCESSED_VIS_PORT caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false

