#!/bin/bash

pkill gst-launch-1.0


gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! \
v4l2convert ! queue ! \
v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
'video/x-h264,level=(string)4.2,profile=(string)baseline' ! \
tcpserversink host=0.0.0.0 port=5000

# Old pipeline WITH rtp that's not being used but still runs

gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! \
v4l2convert ! queue ! \
v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
'video/x-h264,level=(string)4.2,profile=(string)baseline' ! \
h264parse ! rtph264pay ! queue ! \
tcpserversink host=0.0.0.0 port=5000

# Notes about extra controls : https://docs.qualcomm.com/bundle/publicresource/topics/80-70014-50/v4l2h264enc.html