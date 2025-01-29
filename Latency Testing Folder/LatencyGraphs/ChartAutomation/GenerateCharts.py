import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import os

# Define the directory path containing CSV files
directory_path = '/home/digital101/Onboard_VS_Streaming_RPI/Latency Testing Folder/LatencyGraphs/exportedCSVFiles/'

# Define the mapping of old categories to new categories
category_replacements = {
    "RPI --> Laptop ": "Sending Frames to Laptop",
    "Laptop --> Process Frames": "Laptop receive and transmit back to RPI",
    "Laptop processed frames ---> RPI Display": "RPI Recieve From Laptop"
}

# Iterate through each file in the directory
for filename in os.listdir(directory_path):
    filepath = os.path.join(directory_path, filename)

    if os.path.isfile(filepath) and filename.endswith('.csv'):
        print(f"Processing file: {filename}")

        # Load the CSV file
        df = pd.read_csv(filepath, header=None)

        # Replace the old category names with the new ones in the first column
        df.iloc[:, 0] = df.iloc[:, 0].replace(category_replacements)

        # Define the updated categories
        categories = list(category_replacements.values())

        # Initialize a dictionary to store grouped data
        grouped_data = {}

        # Loop through categories and extract the relevant data
        for i, category in enumerate(categories):
            # str.strip() : removes any whitespace before or after the category elements 
            matches = df[df.iloc[:, 0].str.strip() == category.strip()]
            if matches.empty:
                print(f"Category '{category}' not found in the DataFrame.")
                continue

            # Find the start index for the category
            start_index = matches.index[0] + 2  # Skip category and column header row

            # Find the end index for the category
            if i + 1 < len(categories):
                next_matches = df[df.iloc[:, 0].str.strip() == categories[i + 1].strip()]
                end_index = next_matches.index[0] - 1 if not next_matches.empty else len(df)
            else:
                end_index = len(df)

            # Extract the data for the category
            group_df = df.iloc[start_index:end_index].reset_index(drop=True)
            group_df.columns = ["element", "median", "mean", "stdev", "var"]  # Assign appropriate column names

            # Remove the row where element is "Total Time"
            group_df = group_df[group_df["element"] != "Total Time"]

            grouped_data[category] = group_df

        # Create a single figure with subplots
        fig, axs = plt.subplots(1, 3, figsize=(18, 6))  # 1 row, 3 columns of subplots

        # Loop through categories and plot each bar chart in its respective subplot
        for i, category in enumerate(categories):
            data = grouped_data.get(category, pd.DataFrame())
            if data.empty:
                print(f"No data found for category {category}")
                continue

            elements = data["element"]
            means = data["mean"].astype(float)  # Ensure means are float

            # Use a matplotlib color map for unique colors for each element
            colors = cm.tab20.colors  # Tab20 colormap from matplotlib

            # For cases with more than 20 elements, cycle through the colors
            if len(elements) > len(colors):
                colors = [colors[i % len(colors)] for i in range(len(elements))]

            # Plot on the respective subplot (axs[i] corresponds to each subplot)
            axs[i].bar(elements, means, color=colors[:len(elements)])

            # Customize the chart for each subplot
            axs[i].set_ylabel("Mean Time (ms)")
            axs[i].set_title(f"Mean Latency Times for : {category}")
            axs[i].set_xlabel("Individual GStreamer Elements")
            axs[i].tick_params(axis="x", rotation=45)  # Rotate labels for readability

        # Automatically adjust layout for better spacing
        plt.tight_layout()

        # Remove the ".csv" from the filename
        file_name_without_extension = filename.split(".")[0]
        
        # Save the plot as a PNG file
        plt.savefig(f"{directory_path}{file_name_without_extension}.png")
        # plt.show()

        print(f"Graph for {filename} saved as {file_name_without_extension}.png\n")
