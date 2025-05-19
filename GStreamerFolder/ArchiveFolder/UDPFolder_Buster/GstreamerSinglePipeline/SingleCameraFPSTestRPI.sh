raspivid -t 0 -n -w 640 -h 480 -fps 60 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.174 port=5000 &

gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,width=640,height=480,framerate=60/1,format=NV12 ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.1,profile=(string)baseline' ! h264parse ! rtph264pay ! udpsink host=172.17.141.174 port=5000 &

gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5003 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false
