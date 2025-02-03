import matplotlib.pyplot as plt
import numpy as np

# Define X-axis labels 
resolutions = ["640x480", "1280x720", "1920x1080"]

# TCP Latency Data
tcp_latency = {
    "Raspberry Pi to Laptop": [10.144, 31.968, 233.078],
    "Laptop to Raspberry Pi": [2.553, 5.651, 97.923],
    "Raspberry Pi display images from laptop": [3.002, 9.305, 54.712],
}

# UDP Latency Data
udp_latency = {
    "Raspberry Pi to Laptop": [12.884, 25.007, 65.728],
    "Laptop to Raspberry Pi": [5.610, 19.888, 23.355],
    "Raspberry Pi display images from laptop": [4.927, 19.011, 80.260],
}

x = np.arange(len(resolutions))  # X-axis positions
width = 0.25  # Bar width

fig, axs = plt.subplots(1, 2, figsize=(14, 6), layout="constrained")  

colors = ["#1f77b4", "#ff7f0e", "#2ca02c"]  # Blue, Orange, Green

# Plot TCP data (Left subplot)
for i, (category, values) in enumerate(tcp_latency.items()):
    offset = width * i  
    rects = axs[0].bar(x + offset, values, width, label=category, color=colors[i])
    axs[0].bar_label(rects, padding=2)

axs[0].set_title("TCP Latency")
axs[0].set_ylabel("Latency (ms)")
axs[0].set_xticks(x + width, resolutions)
axs[0].legend(loc="upper left")
axs[0].set_ylim(0, max(max(tcp_latency.values())) + 20)

# Plot UDP data (Right subplot)
for i, (category, values) in enumerate(udp_latency.items()):
    offset = width * i  
    rects = axs[1].bar(x + offset, values, width, label=category, color=colors[i])
    axs[1].bar_label(rects, padding=2)

axs[1].set_title("UDP Latency")
axs[1].set_ylabel("Latency (ms)")
axs[1].set_xticks(x + width, resolutions)
axs[1].legend(loc="upper left")
axs[1].set_ylim(0, max(max(udp_latency.values())) + 20)

plt.savefig("TCP_UDP_Latency.png")

# Show the plot
plt.show()
