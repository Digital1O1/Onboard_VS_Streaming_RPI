#!/bin/bash
# IMPORTANT NOTE: RUN STREAMING PROGRAM ON LINUX LAPTOP FIRST OR WINDOW NAMING WILL BE INCORRECT

# Countdown signal file
COUNTDOWN_FILE="/tmp/countdown_signal.txt"

# Ensure the command file exists
if [ ! -f "$COUNTDOWN_FILE" ]; then
    touch "$COUNTDOWN_FILE"
fi

# Function to clean up background processes
cleanup() {
    echo "Cleaning up..."
    kill "$CAMERA1_PID" "$CAMERA2_PID" "$PROCESSED_PID" "$PROFUSION_PID" &> /dev/null
    exit 0
}

# Trap SIGINT (Ctrl + C) and call cleanup
trap cleanup SIGINT

# Export display for graphical applications
export DISPLAY=:0

# Check dependencies
for cmd in xdotool gst-launch-1.0; do
    if ! command -v $cmd &> /dev/null; then
        echo "Error: $cmd not found. Please install it."
        exit 1
    fi
done

# Host and port configuration
HOST_LAPTOP="172.17.140.56"
PORT_CAM1=5000
PORT_CAM2=5001
HOST_RECEIVER="172.17.141.124"
PORT_PROCESSED=5003
PORT_PROFUSION=5004

# Start the first camera stream
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
    capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! \
    v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
    'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! \
    queue ! udpsink host="172.17.141.124" port="5000" &

# Start the second camera stream
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! \
    capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! \
    v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
    'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! \
    queue ! udpsink host=$HOST_LAPTOP port=$PORT_CAM2 &

# Start receiving and displaying processed frames
gst-launch-1.0 udpsrc address=$HOST_RECEIVER port=$PORT_PROCESSED caps=application/x-rtp ! \
    rtph264depay ! h264parse ! queue ! v4l2h264dec ! \
    xvimagesink sync=false &

PROCESSED_PID=$!
echo "The PID of PROCESSED_FRAMES is: $PROCESSED_PID"

# Start receiving and displaying profusion frames
gst-launch-1.0 udpsrc address=$HOST_RECEIVER port=$PORT_PROFUSION caps=application/x-rtp ! \
    rtph264depay ! h264parse ! queue ! v4l2h264dec ! \
    xvimagesink sync=false &

PROFUSION_PID=$!
echo "The PID of PROFUSION_FRAMES is: $PROFUSION_PID"

# Monitor the command file for new commands
echo "Monitoring $COUNTDOWN_FILE for new commands..."
tail -f -n0 "$COUNTDOWN_FILE" | while read -r command; do
    if [ -n "$command" ]; then
        echo "Executing: $command"
        eval "$command"
    fi
done

# Wait for any background processes to finish
wait
