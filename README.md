<!-- # GStreamer Pipelines That are being TESTED So Far
## RPI 
```bash
pkill raspivid
pkill gst-launch-1.0
export DISPLAY=:0
gst-launch-1.0 -vvvvv v4l2src device=/dev/video0 ! video/x-raw,width=1920,height=1080,framerate=30/1,format=I420,colorimetry=bt601,interlace-mode=progressive ! omxh264enc ! video/x-h264,profile=baseline ! h264parse ! rtph264pay config-interval=-1 ! udpsink host=172.17.141.30 port=5001 &
gst-launch-1.0 -vvvvv v4l2src device=/dev/video1 ! video/x-raw,width=1920,height=1080,framerate=30/1,format=I420,colorimetry=bt601,interlace-mode=progressive ! omxh264enc ! video/x-h264,profile=baseline ! h264parse ! rtph264pay config-interval=-1 ! udpsink host=172.17.141.30 port=5002
```
## Linux Laptop
```bash
gst-launch-1.0 udpsrc address=172.17.141.30 port=5001 caps=application/x-rtp ! queue ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink &
gst-launch-1.0 udpsrc address=172.17.141.30 port=5002 caps=application/x-rtp ! queue ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink
```
# GStreamer Pipelines That AREN'T Being Used -->

# Current goals for manuscript
### Profusion stuff
- Inject mouse with tracer
- Watch transfer be absorbed into mouse


# GStreamer install 

```bash
sudo apt-get install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio gstreamer1.0-libcamera

```


# Buster GStreamer Pipelines
 
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
## Bookworm GStreamer Pipelines
```bash
# Test output video
gst-launch-1.0 libcamerasrc ! video/x-raw,width=640,height=480,framerate=30/1 ! autovideosink


 # This currently works
 gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.56 port=5001

 # Display camera0
 gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.56 port=5001

 # Display camera1
 gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.56 port=5001

 # Currently testing the following pipeline
gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! v4l2h264enc ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay! udpsink host=172.17.140.56 port=5001

# Currently testing latnecy stuff here

GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency" gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.56 port=5001

```

## Notes about latency 

```bash
# Currently using the following to detrmine latency
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency" gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=640,
height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 
'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.56 port=5001

# Currently TESTING
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency;stats" gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.56 port=5001



# The current output
TRACE GST_TRACER :0:: latency, 
    src-element-id=(string)0x55a4ea6000, 
    src-element=(string)libcamerasrc0, 
    src=(string)src, 
    sink-element-id=(string)0x55a4e427b0, 
    sink-element=(string)udpsink0, 
    sink=(string)sink, 
    time=(guint64)11470288, 
    ts=(guint64)86626707237;


0:00:20.092869570  5929   
0x557b61ac30 TRACE             
GST_TRACER :0:: message, 
thread-id=(guint64)367142218800, 
ts=(guint64)20092829700, 
element-ix=(uint)9, # This would be the 9th element in the gstreamer pipeline
name=(string)state-changed, 
structure=(structure)"GstMessageStateChanged\,\ old-state\=\(GstState\)playing\,\ new-state\=\(GstState\)paused\,\ pending-state\=\(GstState\)ready\;";
```
### Key components
1) `src-element` and `sink-element`
   - src-element: 
     - The element where data originates.
       In this example: `libcamerasrc0` (the camera source).
       sink-element: The element where data flows.
       In this example: udpsink0 (the network sink).
2) time
    - This represents the latency in nanoseconds (ns) for data to travel from the source element to the sink element.
    - Example: time=(guint64)11470288 translates to 11.47 milliseconds (ms).
3) ts
   - This is a timestamp (ts=(guint64)86626707237) in nanoseconds from the pipeline clock.
   - It represents the time when the latency was measured.

### What we're measuring
- The end-to-end latency between:
    - The source (libcamerasrc, capturing video frames from the camera).
    - The sink (udpsink, sending the encoded frames over the network).

- This measures how long it takes for a frame to:
    - Be captured by the camera.
    - Be processed through the pipeline.
    - Be handed off to the udpsink for network transmission.

### Steps to improve latency measurement 
- Break it into segments
- To insert probes
  - `gst_pad_add_probe`
- To measure individual latencies between elements like
    `libcamerasrc -> v4l2convert`
    `v4l2h264enc -> rtph264pay`
- Analyze Per-element latency
  - Use `GST_TRACERS` for more debugging info
  ```bash
  GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency;stats" gst-launch-1.0 ...
  ```
<br>

## Useful References
- https://gist.github.com/jj1bdx/f3500685a6fca82f1cbea5164ba898a3#send-a-test-video-with-h264-rtp-stream
- [latency-clock](https://github.com/stb-tester/latency-clock)
-  [GStreamerLatencyPlotter](https://github.com/podborski/GStreamerLatencyPlotter)