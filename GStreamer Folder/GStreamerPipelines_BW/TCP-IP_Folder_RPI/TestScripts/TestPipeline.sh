# RPI

#!/bin/bash

pkill gst-launch-1.0

# gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
# capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! \
# v4l2convert ! queue ! \
# v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
# 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! \
# h264parse ! rtph264pay ! queue ! \
# tcpserversink host=0.0.0.0 port=5000

# Send to laptop
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! tcpserversink host=172.17.140.24 port=7001 &

# Recieve from laptop

# Testing

# Use laptop IP address
gst-launch-1.0 -v tcpclientsrc host=172.17.140.128 port=6000 ! \
application/x-rtp,encoding-name=H264,payload=96 ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false
