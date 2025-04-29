gst-launch-1.0 -v tcpclientsrc host=172.17.140.56 port=5003 !   h264parse ! avdec_h264 ! videoconvert ! autovideosink sync=false
