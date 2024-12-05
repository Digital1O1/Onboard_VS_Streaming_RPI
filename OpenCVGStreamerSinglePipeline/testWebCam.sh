#!/bin/bash


gst-launch-1.0 udpsrc port=5001 caps="application/x-rtp, encoding-name=H264, payload=96" ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink