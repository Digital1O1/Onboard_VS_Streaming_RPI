#!/usr/bin/env bash

# THIS DOESN'T WORK AND YOU HAVE TO RUN THE PI PIPELINES INDIVIDUALLY 

# tcpclientsrc host : PI
# tcpserversink host : Laptop
gst-launch-1.0 tcpclientsrc host=172.17.140.24 port=7001 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! rtph264depay ! h264parse \
    ! openh264dec \
    ! tee name=t ! queue ! autovideosink t. ! openh264enc ! rtph264pay ! rtpstreampay \
    ! tcpserversink host=172.17.140.56 port=7002 &

# pi receive encoded video from laptop
# Use laptop IP 
gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.56 port=7002 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! rtph264depay ! h264parse \
    ! openh264dec ! autovideosink

