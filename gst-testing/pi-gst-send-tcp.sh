#!/usr/bin/env bash

# send video test source
# Use PI address 
gst-launch-1.0 -vvv videotestsrc is-live=true ! openh264enc ! rtph264pay ! rtpstreampay ! tcpserversink host=$(hostname -I) port=7001


