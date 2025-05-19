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

echo "--------------------------------------------------------------------------------------------------"
echo "Running latency plotter..."

# echo "$JS_SCRIPT_PATH" -i "$RAW_DATA_LOG" --csv /home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/RawDataRpiToLaptop.csv
# echo "$JS_SCRIPT_PATH" -i "$PROCESSED_DATA_LOG" --csv /home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/SingleStreamUDPRpi/ProcessedDataFromLaptop.csv


node "$JS_SCRIPT_PATH" -i "$RAW_DATA_LOG" --csv "$RAW_DATA_OUTPUT"


node "$JS_SCRIPT_PATH" -i "$PROCESSED_DATA_LOG" --csv "$PROCESSED_DATA_OUTPUT"
