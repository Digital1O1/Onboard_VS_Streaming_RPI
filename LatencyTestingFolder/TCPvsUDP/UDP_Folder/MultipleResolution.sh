#!/bin/bash

pkill raspivid
pkill gst-launch-1.0

export DISPLAY=:0

# Predefined values
RESOLUTION_WIDTH_L=640
RESOLUTION_HEIGHT_L=480
BITRATE_L=10000000

RESOLUTION_WIDTH_H=1280
RESOLUTION_HEIGHT_H=720
BITRATE_H=12000000

SET_FPS=30/1
IR_CAMERA=0
VISIBLE_CAMERA=1
LAPTOP_PORT_VIS=7001
LAPTOP_PORT_IR=7002
RPI_PORT_VIS=5003
RPI_PORT_IR=5004
LAPTOP_IP=172.17.140.56
RPI_IP=172.17.140.240
LOG_SAVE_PATH_VIS="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/UDP_Folder/test_rpi1.log"
LOG_SAVE_PATH_IR="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/UDP_Folder/test_rpi2.log"

# Ask for user input
read -p "Enter [1] to use 640x480 or [2] to use 720x1280 : " userInput

if [ "$userInput" == "1" ]; then
    echo "Loading parameters for 640x480"
    RESOLUTION_WIDTH=$RESOLUTION_WIDTH_L
    RESOLUTION_HEIGHT=$RESOLUTION_HEIGHT_L
    BITRATE=$BITRATE_L
elif [ "$userInput" == "2" ]; then
    echo "Loading parameters for 720x1280"
    RESOLUTION_WIDTH=$RESOLUTION_WIDTH_H
    RESOLUTION_HEIGHT=$RESOLUTION_HEIGHT_H
    BITRATE=$BITRATE_H
else
    echo "INVALID INPUT"
    exit 1
fi

####### Sender pipelines
raspivid -t 0 -n -w ${RESOLUTION_WIDTH} -h ${RESOLUTION_HEIGHT} -fps ${SET_FPS} -b ${BITRATE} -o - -cs ${VISIBLE_CAMERA} | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=${LAPTOP_IP} port=${LAPTOP_PORT_IR} &

raspivid -t 0 -n -w ${RESOLUTION_WIDTH} -h ${RESOLUTION_HEIGHT} -fps ${SET_FPS} -b ${BITRATE} -o - -cs ${IR_CAMERA} | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=${LAPTOP_IP} port=${LAPTOP_PORT_VIS} &

####### Receive pipelines
gst-launch-1.0 -v udpsrc address=${RPI_IP} port=${RPI_PORT_VIS} caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false &

gst-launch-1.0 -v udpsrc address=${RPI_IP} port=${RPI_PORT_IR} caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false

