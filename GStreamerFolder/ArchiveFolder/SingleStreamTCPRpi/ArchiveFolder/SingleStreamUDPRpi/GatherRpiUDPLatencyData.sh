#!/usr/bin/env bash

echo "RPI Pipeline started"

# Define paths for convenience
JS_SCRIPT_PATH="/home/pi/GStreamerLatencyPlotter/main.js"

# Input Data Logs
RAW_DATA_LOG="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/RawDataRPI.log"
PROCESSED_DATA_LOG="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/ProcessedDataLaptop.log"

# Output Data Logs
RAW_DATA_OUTPUT="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/RawDataRpiToLaptop.csv"
PROCESSED_DATA_OUTPUT="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/ProcessedDataFromLaptop.csv"


# Define the duration to run the pipelines (in seconds)
PIPELINE_RUNTIME=15  # Change this to your desired duration

# Start the sender pipeline in the background
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE=/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/RawDataRPI.log \
gst-launch-1.0 libcamerasrc \
    ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 \
    ! v4l2convert \
    ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" \
    ! 'video/x-h264,level=(string)4.1' \
    ! h264parse \
    ! rtph264pay \
    ! queue \
    ! udpsink host=172.17.140.56 port=7001 &

# The $! saves the most recent executed background pprocess 
SENDER_PID=$!

# Start the receiver pipeline in the background
GST_DEBUG_COLOR_MODE=off GST_TRACERS="latency(flags=element)" GST_DEBUG=GST_TRACER:7 GST_DEBUG_FILE=/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/ProcessedDataLaptop.log \
gst-launch-1.0  \
    udpsrc address=172.17.140.24 port=7002 \
    ! application/x-rtp-stream,encoding-name=H264 \
    ! rtpstreamdepay \
    ! queue \
    ! rtph264depay \
    ! queue \
    ! h264parse \
    ! queue \
    ! openh264dec \
    ! queue \
    ! autovideosink &

# Save the PID of the receiver process
RECEIVER_PID=$!

# Run the pipelines for the specified duration
echo "Pipelines running for $PIPELINE_RUNTIME seconds..."
sleep $PIPELINE_RUNTIME

# Terminate the pipelines
echo "Stopping pipelines..."
kill $SENDER_PID $RECEIVER_PID

# Wait for the processes to fully terminate
wait $SENDER_PID $RECEIVER_PID 2>/dev/null

# Run the JavaScript program

echo "--------------------------------------------------------------------------------------------------"
echo "Running latency plotter..."

# echo "$JS_SCRIPT_PATH" -i "$RAW_DATA_LOG" --csv /home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/RawDataRpiToLaptop.csv
# echo "$JS_SCRIPT_PATH" -i "$PROCESSED_DATA_LOG" --csv /home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/ProcessedDataFromLaptop.csv


node "$JS_SCRIPT_PATH" -i "$RAW_DATA_LOG" --csv "$RAW_DATA_OUTPUT"


node "$JS_SCRIPT_PATH" -i "$PROCESSED_DATA_LOG" --csv "$PROCESSED_DATA_OUTPUT"

