# Pipeline testing for latency

# Current testing 
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency(element)" GST_DEBUG_FILE=./latency.log gst-launch-1.0 -vvv libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=640,height=480,format=I420 ! queue ! openh264enc ! queue ! rtph264pay ! queue ! rtpstreampay ! queue ! tcpserversink host=172.17.140.24 port=7001




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




# To measure end to end latnecy 
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

gst-launch-1.0 tcpclientsrc host=172.17.140.24 port=7001 \
    ! application/x-rtp,media=video,encoding-name=H264,payload=96 \
    ! rtpstreamdepay \
    ! rtph264depay \
    ! h264parse \
    ! avdec_h264 \
    ! timeoverlay silent=false \
    ! autovideosink
