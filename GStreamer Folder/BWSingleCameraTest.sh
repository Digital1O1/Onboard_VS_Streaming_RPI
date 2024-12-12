# gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=1280,height=720,format=NV12 ! v4l2convert ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4' ! h264parse ! queue ! rtph264pay ! appsink

gst-launch-1.0 libcamerasrc ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! queue ! v4l2convert ! videoconvert ! video/x-raw,format=BGR ! autovideosink

# This works
 gst-launch-1.0 libcamerasrc ! video/x-raw,width=1280,height=720,framerate=30/1 ! videoconvert ! autovideosink