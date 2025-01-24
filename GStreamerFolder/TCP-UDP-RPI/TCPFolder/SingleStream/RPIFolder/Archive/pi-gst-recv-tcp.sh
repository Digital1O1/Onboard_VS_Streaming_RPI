#!/usr/bin/env bash

# pi receive encoded/processed video from laptop
# Use laptop IP 

# Works 
gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.56 port=7002 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! rtph264depay ! h264parse \
    ! openh264dec ! autovideosink



# Testing in general
gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.56 port=7002 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! rtph264depay ! h264parse \
    ! v4l2h264dec ! autovideosink


# Latency testing
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE=/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/gst-testing-pi/RPI_TCP_Manuscript/SingleStream/RPIFolder/RecProcessedFramesLaptop/ProcessedLaptopToRPI.log gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.56 port=7002 \
! application/x-rtp-stream,encoding-name=H264 \
! rtpstreamdepay \
! rtph264depay ! h264parse \
! openh264dec ! autovideosink