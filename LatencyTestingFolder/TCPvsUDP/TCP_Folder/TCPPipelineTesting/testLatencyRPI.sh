#!/usr/bin/env bash

pkill gst-launch-1.0
echo "Local Test Pipeline started"

LOG_PATH="/home/pi/Onboard_VS_Streaming_RPI/LatencyTestingFolder/TCPvsUDP/TCP_Folder/TCPPipelineTesting/LocalTest.log"

GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" \
GST_DEBUG=GST_TRACER:7 \
GST_DEBUG_FILE="$LOG_PATH" \
gst-launch-1.0 videotestsrc is-live=true pattern=ball \
    ! video/x-raw,width=1920,height=1080,format=NV12,framerate=30/1 \
    ! v4l2convert \
    ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" \
    ! 'video/x-h264,level=(string)4.1' \
    ! h264parse \
    ! rtph264pay \
    ! rtph264depay \
    ! h264parse \
    ! v4l2h264dec \
    ! autovideosink

