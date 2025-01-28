import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm  
import os 

directory_path = '/home/digital101/Onboard_VS_Streaming_RPI/Latency Testing Folder/LatencyGraphs/exportedCSVFiles/'

print("=================== Printing file paths ===================")
for filename in os.listdir(directory_path):
    filepath = os.path.join(directory_path, filename)
    if os.path.isfile(filepath):
        # print(filename)
        
        # Load the CSV file
        file_path = directory_path + filename + ".csv"

        # print("File path : ", file_path)

        # string.split(separator, maxsplit)
        # maxsplit : Optional. Specifies how many splits to do. Default value is -1, which is "all occurrences"
        parts = file_path.split("/")


        partString = parts[7] # This will return 'TCP 60 Seconds 1920x1080.csv'
        file_name = partString.split(".")[0] # This will return 'TCP 60 Seconds 1920x1080'
        #csvPortion = partString.split(".")[1] # This will return 'csv'
        print(file_name)
        #print(csvPortion)


print("============================================================")

        
