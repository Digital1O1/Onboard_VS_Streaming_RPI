
# Resolutions to test
# 640x480   @ 1-3 Mbps   | Originally set to 2Mbps and checks out
# 1280x720  @ 3-6 Mbps   | Tested at 4Mbps and checks out
# 1920x1080 @ 6-10 Mbps  | Tested at 8Mbps and checks out | At 10 Mbps got the best picture quality but very noticeable lag


# REMEMBER TO ALSO CHANGE THE BIT RATE ON THE LAPTOP SIDE

#!/bin/bash

# Kill previous processes
pkill raspivid
pkill gst-launch-1.0

# Get CLI parameters being passed in 
RESOLUTION_WIDTH=$1
RESOLUTION_HEIGHT=$2
USER_BITRATE=$3
LOG_FILE_PATH=$4

BITRATE=$((USER_BITRATE * 1000))

# Configuration
# RESOLUTION_WIDTH=640
# RESOLUTION_HEIGHT=480
SET_FPS=30
# BITRATE=2000
VISIBLE_CAMERA=/dev/video1
LAPTOP_PORT=7001
LAPTOP_IP=172.17.140.252
LOG_SAVE_PATH="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/TCP_Folder/TCPPipelineTesting/${LOG_FILE_PATH}"

# Ensure log directory exists
mkdir -p "$(dirname "$LOG_SAVE_PATH")"

# Run pipeline with logging
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="$LOG_SAVE_PATH" \
gst-launch-1.0 -v v4l2src device=${VISIBLE_CAMERA} ! \
"video/x-raw,width=${RESOLUTION_WIDTH},height=${RESOLUTION_HEIGHT},framerate=${SET_FPS}/1" ! \
x264enc tune=zerolatency bitrate=${BITRATE} speed-preset=ultrafast ! \
h264parse ! rtph264pay config-interval=-1 pt=96 ! rtpstreampay ! \
tcpserversink host=0.0.0.0 port=${LAPTOP_PORT}







# Use this and don't specify host IP address since RPI is acting as a server and allowing any device to connnect to it

#raspivid -t 0 -n -w $RESOLUTION_WIDTH -h $RESOLUTION_HEIGHT -fps $SET_FPS -b $BITRATE -o - -cs $VISIBLE_CAMERA | \
#gst-launch-1.0 -v fdsrc ! \
#h264parse ! rtph264pay config-interval=-1 pt=96 ! rtpstreampay ! \
#tcpserversink host=0.0.0.0 port=$LAPTOP_PORT
