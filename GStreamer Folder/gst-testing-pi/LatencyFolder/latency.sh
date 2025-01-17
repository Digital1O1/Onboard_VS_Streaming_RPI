# Reference link for latency : https://gstreamer.freedesktop.org/documentation/additional/design/tracing.html?gi-language=c

# Pipeline testing for latency

# Use to measure individual elements in pipeline
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency(element)" GST_DEBUG_FILE=./element-latency.log gst-launch-1.0 -vvv libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 ! queue ! openh264enc ! queue ! rtph264pay ! queue ! rtpstreampay ! queue ! tcpserversink host=172.17.140.24 port=7001




# Can remove *.5 from GST_DEBUG if you want 
# GST_DEBUG="*:5,GST_TRACER:7" GST_TRACERS="latency(flags=element+pipeline)" GST_DEBUG_FILE=./latency.log gst-launch-1.0 -vvv libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 ! queue ! openh264enc ! queue ! rtph264pay ! queue ! rtpstreampay ! queue ! tcpserversink host=172.17.140.24 port=7001



# Add clocksync to measure specific points to log timestamps and compare latency 

gst-launch-1.0 -vvv libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" \
    ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 \
    ! queue \
    ! clocksync \
    ! openh264enc \
    ! queue \
    ! rtph264pay \
    ! queue \
    ! clocksync \
    ! rtpstreampay \
    ! queue \
    ! clocksync \
    ! tcpserversink host=172.17.140.24 port=7001




################## To measure end to end latnecy ########################
# Sender
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" \
    ! timeoverlay \
    ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 \
    ! queue \
    ! openh264enc \
    ! queue \
    ! rtph264pay \
    ! queue \
    ! tcpserversink host=172.17.140.24 port=7001


# Recieve 

gst-launch-1.0 tcpclientsrc host=172.17.140.24 port=7001 ! \
application/x-rtp-stream,encoding-name=H264 ! \
rtpstreamdepay ! rtph264depay ! h264parse ! openh264dec ! \
timeoverlay ! tee name=t ! queue ! autovideosink t. ! \
openh264enc ! rtph264pay ! rtpstreampay ! \
tcpserversink host=172.17.140.128 port=7002
