#!/usr/bin/env bash
pkill gst-launch-1.0
echo "RPI Pipelines started"

# Variables for log file names
LOG_FILENAME_SEND="SendToLaptop.log"
LOG_FILENAME_RECEIVE="RecieveFromLaptop.log"

LOG_BASE_PATH="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStream"

FULL_LOG_PATH_SEND="${LOG_BASE_PATH}/${LOG_FILENAME_SEND}"
FULL_LOG_PATH_RECEIVE="${LOG_BASE_PATH}/${LOG_FILENAME_RECEIVE}"

# echo $FULL_LOG_PATH_SEND
# echo $FULL_LOG_PATH_RECEIVE


# Start the sender pipeline
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/TCP-UDP-RPI/UDPFolder/SingleStream/SendToLaptop.log" \
gst-launch-1.0 libcamerasrc \
    ! capsfilter caps=video/x-raw,width=1920,height=1080,format=NV12 \
    ! v4l2convert \
    ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" \
    ! 'video/x-h264,level=(string)4.1' \
    ! h264parse \
    ! rtph264pay \
    ! queue \
    ! udpsink host=172.17.140.56 port=5000 &

# The $! saves the most recent executed background process
# SENDER_PID=$!

# Start the receiver pipeline reciving the processed frames from the laptop

# Original
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/TCP-UDP-RPI/UDPFolder/SingleStream/RecieveFromLaptop.log" \
gst-launch-1.0 \
    udpsrc address=172.17.140.24 port=7002 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! queue \
    ! rtph264depay \
    ! queue \
    ! h264parse \
    ! queue \
    ! openh264dec \
    ! queue \
    ! autovideosink 

# Testing 
# GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/TCP-UDP-RPI/UDPFolder/SingleStream/RecieveFromLaptop.log" gst-launch-1.0     udpsrc address=172.17.140.24 port=7002     ! application/x-rtp,encoding-name=H264     ! queue     ! rtph264depay     ! queue     ! h264parse     ! queue     ! openh264dec     ! queue     ! autovideosink