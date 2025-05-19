#!/usr/bin/env bash

echo "RPI Pipeline started"

# Path to where the JS script is at
JS_SCRIPT_PATH="/home/pi/GStreamerLatencyPlotter/main.js"

# Input Data Logs
RAW_DATA_LOG="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/UDPFolder/SingleStreamUDPRpi/RawDataRPI.log"
PROCESSED_DATA_LOG="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/UDPFolder/SingleStreamUDPRpi/ProcessedDataFromLaptopUDP.log"

# Output Data Logs
RAW_DATA_OUTPUT="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/UDPFolder/SingleStreamUDPRpi/RawDataRpiToLaptop.csv"
PROCESSED_DATA_OUTPUT="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/UDPFolder/SingleStreamUDPRpi/RecievedProcessedDataFromLaptop.csv"

node "$JS_SCRIPT_PATH" -i "$RAW_DATA_LOG" --csv "$RAW_DATA_OUTPUT"
node "$JS_SCRIPT_PATH" -i "$PROCESSED_DATA_LOG" --csv "$PROCESSED_DATA_OUTPUT"

