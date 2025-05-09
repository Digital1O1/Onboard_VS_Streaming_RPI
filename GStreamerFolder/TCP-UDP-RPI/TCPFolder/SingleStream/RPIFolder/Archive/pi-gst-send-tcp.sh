#!/usr/bin/env bash

# send video test source
# Use PI address for tcpserversink

# Orignal
#gst-launch-1.0 -vvv videotestsrc is-live=true ! openh264enc ! rtph264pay ! rtpstreampay ! tcpserversink host=172.17.140.24 port=7001

# Testing but still works
# gst-launch-1.0 -vvv libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" \
#     ! timeoverlay \
#     ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 \
#     ! queue \
#     ! openh264enc \
#     ! queue \
#     ! rtph264pay \
#     ! queue \
#     ! rtpstreampay \
#     ! queue \
#     ! tcpserversink host=172.17.140.24 port=7001


############################# Use to measure latency #############################

# Option 1
# GST_DEBUG="GST_LATENCY:7" gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" \
#     ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 \
#     ! queue \
#     ! openh264enc \
#     ! queue \
#     ! rtph264pay \
#     ! queue \
#     ! rtpstreampay \
#     ! queue \
#     ! tcpserversink host=172.17.140.24 port=7001


# Option 2 

# GST_DEBUG="GST_TRACER:7" GST_TRACERS=latency GST_DEBUG_FILE=./RPItoLaptop-latency.log gst-launch-1.0 -vvv libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 ! queue ! openh264enc ! queue ! rtph264pay ! queue ! rtpstreampay ! queue ! tcpserversink host=172.17.140.24 port=7001


# Option 3 using GStreamerLatencyPlotter
# This currently works and is being used 1/23/25


GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE=/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/gst-testing-pi/RPI_TCP_Manuscript/SingleStream/RPIFolder/SendDataToLaptop/RPIRawStreamToLaptop.log gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 ! queue ! openh264enc ! queue ! rtph264pay ! queue ! rtpstreampay ! queue ! tcpserversink host=$(hostname -I) port=7001

# Being used as reference but this works when using I420 format
#gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! tcpserversink host=172.17.140.24 port=7001