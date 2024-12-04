pkill raspivid
pkill gst-launch-1.0

export DISPLAY=:0

# Send to Linux Laptop
# Put Laptop IP address here
# raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.140.56 port=5000 &

raspivid -t 0 -n -w 640 -h 480 -fps 30 -b 2000000 -o - -cs 1 | gst-launch-1.0  fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.140.56 port=5000 &

# Pipeline to recieve frames from laptop
# Place RPI IP address here
gst-launch-1.0 -v udpsrc address=172.17.141.124 port=5001 caps=application/x-rtp ! \
rtph264depay ! h264parse ! queue ! v4l2h264dec ! autovideosink sync=false




