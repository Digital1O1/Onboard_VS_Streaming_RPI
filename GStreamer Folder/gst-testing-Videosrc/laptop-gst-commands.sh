#!/usr/bin/env bash
# tcpclientsrc host : PI
# tcpserversink host : Laptop

# Works but used for testing
# gst-launch-1.0 tcpclientsrc host=172.17.140.24 port=7001 \
#     ! application/x-rtp-stream,encoding-name=H264 \
#     ! rtpstreamdepay \
#     ! rtph264depay ! h264parse \
#     ! openh264dec \
#     ! tee name=t ! queue ! autovideosink t. ! openh264enc ! rtph264pay ! rtpstreampay \
#     ! tcpserversink host=172.17.140.128 port=7002

# Still testing 

gst-launch-1.0 tcpclientsrc host=172.17.140.24 port=7001     ! application/x-rtp-stream,encoding-name=H264     ! rtpstreamdepay     ! rtph264depay ! h264parse     ! openh264dec     ! tee name=t ! queue ! autovideosink t. ! openh264enc ! rtph264pay ! rtpstreampay     ! tcpserversink host=172.17.140.128 port=7002
