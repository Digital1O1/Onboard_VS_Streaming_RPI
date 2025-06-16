#!/bin/bash

pkill raspivid
pkill gst-launch-1.0


# Notes about UDP bitrate 640 x 480
# 2000  bits  --> not useable
# 4000  bits  --> super grainy and hardly useable
# 8000  bits  --> 56k dial up quality
# 10000 bits  --> Not much better
# 15000 bits  --> Didn't bother due to time
# 20000 bits  --> Sticking with this for testing at this resolution

# Dual UDP 720 x 1280 Bitrate Test Need to test 6 --> 10 MBps
# 8000000 bits --> Testing | Seems like good baseline
# 10000000 | Very useable

# Dual UDP 1920 x 1080 Need to test either 10-16 MBps or 15-25 MBps
# Testing 14

RESOLUTION_WIDTH=1920
RESOLUTION_HEIGHT=1280
# BITRATE_MBPS=$3
# LOG_FILE_PATH=$4

# Convert bitrate bits --> Mbps
# BITRATE=$((BITRATE_MBPS * 1000000))
BITRATE=14000000

# Fixed settings
SET_FPS=30/1
IR_CAMERA=0
VISIBLE_CAMERA=1
LAPTOP_PORT_VIS=7001
LAPTOP_PORT_IR=7002
LAPTOP_IP=172.17.140.56
RPI_IP=172.17.140.240
LOG_SAVE_PATH_VIS="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/UDP_Folder/test_rpi1.log"
LOG_SAVE_PATH_IR="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/UDP_Folder/test_rpi2.log"


export DISPLAY=:0

# Make sure the directory exists first
# mkdir -p "$(dirname "$LOG_SAVE_PATH")"


# Since we're using raspivid, it needs to get 'piped' into the GST_DEBUG stuff


# Sender pipelines
raspivid -t 0 -n -w ${RESOLUTION_WIDTH} -h ${RESOLUTION_HEIGHT} -fps ${SET_FPS} -b ${BITRATE} -o - -cs ${VISIBLE_CAMERA} | \
# GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="$LOG_SAVE_PATH_VIS" \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=${LAPTOP_IP} port=${LAPTOP_PORT_IR} &


raspivid -t 0 -n -w ${RESOLUTION_WIDTH} -h ${RESOLUTION_HEIGHT} -fps ${SET_FPS} -b ${BITRATE} -o - -cs ${IR_CAMERA} | \
# GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="$LOG_SAVE_PATH_IR" \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=${LAPTOP_IP} port=${LAPTOP_PORT_VIS} &


# Recieve pipelines, use RPI IP address here

gst-launch-1.0 -v udpsrc address=${RPI_IP} port=5003 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false &

# visibleCameraPipeline
gst-launch-1.0 -v udpsrc address=${RPI_IP}ort=5004 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false

