#!/usr/bin/env bash

# send video test source
# Use laptop address for udpsink host

GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE=/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/gst-testing-pi/RPI_TCP_Manuscript/SingleStream/RPIFolder/SendDataToLaptop/RPIRawStreamToLaptop.log 

# Currently works to send video to laptop
gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.1' ! h264parse ! rtph264pay ! udpsink host=172.17.140.56 port=5000

# Testing to recieve processed frames from laptop
# Use PI address

gst-launch-1.0 udpsrc address=172.17.140.24 port=7002 \
! application/x-rtp-stream,encoding-name=H264 \
! rtpstreamdepay \
! rtph264depay ! h264parse \
! openh264dec ! autovideosink