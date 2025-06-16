#!/bin/bash

LOG_FILE_PATH=$1

export DISPLAY=:0

LOG_SAVE_PATH="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/UDP_Folder/${LOG_FILE_PATH}"
LAPTOP_PORT=7002

# Ensure the directory exists
mkdir -p "$(dirname "$LOG_SAVE_PATH")"


GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="$LOG_SAVE_PATH" \
gst-launch-1.0 -v \
udpsrc port=${LAPTOP_PORT} caps="application/x-rtp, encoding-name=H264, payload=96" ! \
rtph264depay ! \
h264parse ! \
decodebin ! \
videoconvert ! \
autovideosink sync=false

