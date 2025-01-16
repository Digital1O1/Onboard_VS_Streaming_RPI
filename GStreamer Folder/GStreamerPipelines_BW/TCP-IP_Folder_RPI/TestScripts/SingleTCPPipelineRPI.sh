#!/bin/bash


pkill gst-launch-1.0


#gst-launch-1.0 -vvv videotestsrc ! openh264enc ! rtph264pay ! rtpstreampay ! tcpserversink host=0.0.0.0 port=7001


# This runs DON'T USE

# gst-launch-1.0 -vvv videotestsrc is-live=true ! v4l2h264enc ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! rtph264pay ! rtpstreampay ! tcpserversink port=5000

# This works

gst-launch-1.0 -vvv videotestsrc is-live=true ! v4l2h264enc ! \
'video/x-h264,level=(string)4.2,profile=(string)baseline' ! rtph264pay ! \
rtpstreampay ! tcpserversink host=0.0.0.0 port=5000

