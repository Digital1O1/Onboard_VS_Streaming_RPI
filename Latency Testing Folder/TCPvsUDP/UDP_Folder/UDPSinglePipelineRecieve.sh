gst-launch-1.0 -v \
udpsrc port=5003 caps="application/x-rtp, encoding-name=H264, payload=96" ! \
rtph264depay ! \
h264parse ! \
decodebin ! \
videoconvert ! \
autovideosink sync=false

