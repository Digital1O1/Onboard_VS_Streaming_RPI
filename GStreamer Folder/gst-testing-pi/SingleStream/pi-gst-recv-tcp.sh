#!/usr/bin/env bash

# receive encoded video
# Use laptop IP 
gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.128 port=7002 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! rtph264depay ! h264parse \
    ! openh264dec ! autovideosink

