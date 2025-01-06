pkill gst-launch-1.0

gst-launch-1.0 libcamerasrc ! video/x-raw,format=I420 ! videoconvert ! x264enc tune=zerolatency ! h264parse ! rtph264pay ! tcpserversink host=0.0.0.0 port=5000











# Use libcamera-vid --list-cameras to get camera-name parameter
# Adjust the camera name if needed
# Use Linux IP address here

# gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" !     capsfilter caps=video/x-raw,width=640,height=480,format=NV12 !     v4l2convert ! queue !     v4l2h264enc extra-controls="controls,repeat_sequence_header=1" !   'video/x-h264,level=(string)4.2,profile=(string)baseline' !     h264parse ! rtph264pay pt=96 config-interval=1  !     tcpserversink host=172.17.140.32 port=5001 



# gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
#     capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! \
#     v4l2convert ! queue ! \
#     v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
#     'video/x-h264,level=(string)4.2,profile=(string)baseline' ! \
#     h264parse ! rtph264pay pt=96 ! \
#     tcpserversink host=172.17.140.56 port=5001 &


# Pipeline to receive frames from the laptop over TCP
# Place RPI IP address here
# gst-launch-1.0 -v tcpclientsrc host=172.17.140.56 port=6000 ! \
#     video/x-raw, format=NV12 ! \
#     videoconvert ! autovideosink

