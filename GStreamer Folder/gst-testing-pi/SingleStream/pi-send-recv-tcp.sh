#!/usr/bin/env bash

# send video test source/laptop
# Use PI address 
gst-launch-1.0 -vvv videotestsrc is-live=true ! openh264enc ! rtph264pay ! rtpstreampay ! tcpserversink host=172.17.140.24 port=7001 &

# receive encoded video from laptop
# Use laptop IP 
gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.128 port=7002 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! rtph264depay ! h264parse \
    ! openh264dec ! autovideosink

