# Pipeline notes

## Current RPI UDP pipeline that works

```bash
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,
height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/
x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.32 port=5001
```

gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.32 port=7001 ! rtpstreamdepay ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink


---

# Notes about RPI Pipeline 

### v4l2h264enc

