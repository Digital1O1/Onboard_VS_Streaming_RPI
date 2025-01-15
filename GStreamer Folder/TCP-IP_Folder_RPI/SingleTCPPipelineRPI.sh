#!/bin/bash


pkill gst-launch-1.0

# Send 

gst-launch-1.0 -vvv videotestsrc is-live=true ! v4l2h264enc ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! rtph264pay ! rtpstreampay ! tcpserversink port=7001



# RPI Sending pipeline to Laptop
#gst-launch-1.0 -vvv videotestsrc is-live=true ! openh264enc ! rtph264pay ! rtpstreampay \
#! tcpserversink port=7001




# Experimenting with the following pipeline
# gst-launch-1.0 -vvv videotestsrc is-live=true ! v4l2h264enc ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! rtph264pay ! rtpstreampay ! tcpserversink port=7001