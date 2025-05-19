#!/usr/bin/env bash

echo "RPI TCP Pipeline started"


# Start the sender pipeline in the background
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE=/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/TCP-UDP-RPI/TCPFolder/SingleStream/SendToLaptop.log gst-launch-1.0 -vvv libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! capsfilter caps=video/x-raw,width=1920,height=1080,format=I420 ! queue ! openh264enc ! queue ! rtph264pay ! queue ! rtpstreampay ! queue ! tcpserversink host=$(hostname -I) port=7001 &



# Start the receiver pipeline in the background
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE=/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/TCP-UDP-RPI/TCPFolder/SingleStream/RecieveFromLaptop.log gst-launch-1.0 -vvv tcpclientsrc host=172.17.140.56 port=7002 ! application/x-rtp-stream,encoding-name=H264 ! rtpstreamdepay ! rtph264depay ! h264parse ! openh264dec ! autovideosink 


