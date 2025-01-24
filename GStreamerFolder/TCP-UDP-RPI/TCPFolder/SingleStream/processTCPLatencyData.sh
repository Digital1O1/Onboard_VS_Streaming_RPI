# Check out RunDataProcessing.sh script to automate data processing 
# Can be found here : /home/utsw-bmen-laptop/GStreamerLatencyPlotter/C++/RunDataProcessing.sh


#!/usr/bin/env bash

echo "Processing Laptop Data"

# Path to C++ executable to process data
PROGRAM_TO_PROCESS_DATA_PATH="/home/pi/GStreamerLatencyPlotter/C++/latency_plotter"

# Paths to LOG files to be processed
DATA_TO_BE_PROCESSED_SENDER_RPI="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/TCP-UDP-RPI/UDPFolder/SingleStream/SendToLaptop.log"

DATA_TO_BE_PROCESSED_RECIEVED_FROM_LAPTOP="/home/pi/Onboard_VS_Streaming_RPI/GStreamerFolder/TCP-UDP-RPI/UDPFolder/SingleStream/RecieveFromLaptop.log"

# CSV files to be sent to the work laptop
RPISenderData="/home/pi/GStreamerLatencyPlotter/C++/RPISenderData.csv"
RPIRecievedData="/home/pi/GStreamerLatencyPlotter/C++/RPIRecievedProcessedData.csv"

# Transfer to work laptop
SCP_TRANSFER_PATH="C:\Users\S219800\Desktop\TCP_UDP_GStreamerData\TCP\20Seconds"

echo "---------------------------------------------------"
echo "PROGRAM_TO_PROCESS_DATA_PATH : $PROGRAM_TO_PROCESS_DATA_PATH"
echo "DATA_TO_BE_PROCESSED_SENDER_RPI : $DATA_TO_BE_PROCESSED_SENDER_RPI"
echo "DATA_TO_BE_PROCESSED_RECIEVED_FROM_LAPTOP : $DATA_TO_BE_PROCESSED_RECIEVED_FROM_LAPTOP"
echo "RPISenderData : $RPISenderData"
echo "RPIRecievedData : $RPIRecievedData"
echo "SCP_TRANSFER_PATH : $SCP_TRANSFER_PATH"
echo "---------------------------------------------------"



# Process Data
"$PROGRAM_TO_PROCESS_DATA_PATH" -i "$DATA_TO_BE_PROCESSED_SENDER_RPI" --csv "$RPISenderData"

"$PROGRAM_TO_PROCESS_DATA_PATH" -i "$DATA_TO_BE_PROCESSED_RECIEVED_FROM_LAPTOP" --csv "$RPIRecievedData"





# Send data to work laptop
echo "Latency plotter completed. CSV saved as $RPISenderData"
echo "Transferring CSV file to work laptop...."

sudo scp $RPISenderData s219800@172.17.140.2:$SCP_TRANSFER_PATH
sudo scp $RPIRecievedData s219800@172.17.140.2:$SCP_TRANSFER_PATH

