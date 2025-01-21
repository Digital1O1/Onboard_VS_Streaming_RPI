#!/usr/bin/env bash

# send video test source
# Use PI address 
gst-launch-1.0 -vvv videotestsrc is-live=true ! openh264enc ! rtph264pay ! rtpstreampay ! tcpserversink host=172.17.140.24 port=7001


