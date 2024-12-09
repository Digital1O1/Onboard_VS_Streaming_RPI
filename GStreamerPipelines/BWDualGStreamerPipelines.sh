#!/bin/bash

# Function to clean up background processes
cleanup() {
    pkill -f libcamerasrc
    pkill -f gst-launch-1.0
    exit 0
}

# Trap SIGINT (Ctrl + C) and call cleanup
trap cleanup SIGINT

export DISPLAY=:0

# Function to rename windows using xdotool
rename_window() {
    local pid="$1"
    local new_title="$2"
    sleep 3  # Minimum sleep has to be set to 3 in order for windows to have proper individual names
    WINDOW_ID=$(xdotool search --pid "$pid" | head -n 1)
    if [ -n "$WINDOW_ID" ]; then
        xdotool set_window --name "$new_title" "$WINDOW_ID"
    else
        echo "Window for PID $pid not found."
    fi
}

# Start the first camera stream
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
    capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! \
    v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
    'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! \
    queue ! udpsink host=172.17.140.56 port=5000 &

# Capture PID of the first process
CAMERA1_PID=$!
echo "The PID of Camera 1 is: $CAMERA1_PID"

# Start the second camera stream
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! \
    capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! \
    v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
    'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! \
    queue ! udpsink host=172.17.140.56 port=5001 &

# Capture PID of the second process
CAMERA2_PID=$!
echo "The PID of Camera 2 is: $CAMERA2_PID"

# Start receiving and displaying processed frames
gst-launch-1.0 udpsrc address=172.17.141.124 port=5003 caps=application/x-rtp ! \
    rtph264depay ! h264parse ! queue ! v4l2h264dec ! \
    xvimagesink sync=false &

# Capture PID of the processed frames sink
PROCESSED_PID=$!
echo "The PID of PROCESSED_PID is: $PROCESSED_PID"

# Start receiving and displaying profusion frames
gst-launch-1.0 udpsrc address=172.17.141.124 port=5004 caps=application/x-rtp ! \
    rtph264depay ! h264parse ! queue ! v4l2h264dec ! \
    xvimagesink sync=false &

# Capture PID of the profusion frames sink
PROFUSION_PID=$!
echo "The PID of PROFUSION_PID is: $PROFUSION_PID"

# Rename windows based on their PIDs
rename_window "$PROCESSED_PID" "Test1"
rename_window "$PROFUSION_PID" "Test2"

# Wait for any background processes to finish
wait
