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



RESOLUTION_HEIGHT=$1
RESOLUTION_WIDTH=$2
BITRATE_MBPS=$3
LOG_FILE_PATH=$4

# Convert bitrate bits --> Mbps
BITRATE=$((BITRATE_MBPS * 1000000))

# Fixed settings
SET_FPS=30/1
VISIBLE_CAMERA=1
LAPTOP_PORT=7001
LAPTOP_IP=172.17.140.56
LOG_SAVE_PATH="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/UDP_Folder/${LOG_FILE_PATH}"

export DISPLAY=:0

# Make sure the directory exists first
mkdir -p "$(dirname "$LOG_SAVE_PATH")"


# Since we're using raspivid, it needs to get 'piped' into the GST_DEBUG stuff

raspivid -t 0 -n -w ${RESOLUTION_WIDTH} -h ${RESOLUTION_HEIGHT} -fps ${SET_FPS} -b ${BITRATE} -o - -cs ${VISIBLE_CAMERA} | \
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="$LOG_SAVE_PATH" \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=${LAPTOP_IP} port=${LAPTOP_PORT}

