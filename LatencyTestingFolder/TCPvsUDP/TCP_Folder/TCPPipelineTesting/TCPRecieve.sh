#!/bin/bash
LAPTOP_IP=172.17.140.56
RPI_PORT=5003
LOG_SAVE_PATH="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/TCP_Folder/TCPPipelineTesting/TCP_Recieve_RPI.log"
export DISPLAY=:0

mkdir -p "$(dirname "$LOG_SAVE_PATH")"

# Recieve pipeline


GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="$LOG_SAVE_PATH" \
gst-launch-1.0 -vvvv tcpclientsrc host=${LAPTOP_IP} port=${RPI_PORT} !\
application/x-rtp-stream,encoding-name=H264 ! rtpstreamdepay ! \
rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink sync=false
