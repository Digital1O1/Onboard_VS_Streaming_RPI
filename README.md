# GStreamer Pipelines Currently Being Used

## Single pipeline 

```bash

#!/bin/bash


# Pipeline to send frames to laptop
pkill raspivid
pkill gst-launch-1.0
export DISPLAY=:0
raspivid -t 0 -n -w 640 -h 480 -fps 60 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.174 port=5000 &

# gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,width=640,height=480,framerate=60/1,format=NV12 ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.1,pro$

# Pipeline to recieve frames from laptop
gst-launch-1.0 -v udpsrc address=172.17.140.174 port=5003 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false
```

## Dual pipeline

```bash
#!/bin/bash

pkill raspivid
pkill gst-launch-1.0
export DISPLAY=:0
# irCamera
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 0 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.174 port=5000 &

# visibleCamera
raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.174 port=5001 &

#gst-launch-1.0 v4l2src device=/dev/video0 ! video/x-raw,width=1920,height=1080,framerate=30/1,format=NV12 ! \
#v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.1,profile=(string)b$
#! h264parse ! rtph264pay ! udpsink host=172.17.141.30 port=5000 &

#gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,width=1920,height=1080,framerate=30/1,format=NV12 ! \
#v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.1,profile=(string)b$
#! h264parse ! rtph264pay ! udpsink host=172.17.141.30 port=5001 &


# Command to receive the stream

# irCameraPipeline
gst-launch-1.0 -v udpsrc address=172.17.140.174 port=5002 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false &

# visibleCameraPipeline
gst-launch-1.0 -v udpsrc address=172.17.140.174 port=5003 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false
```
