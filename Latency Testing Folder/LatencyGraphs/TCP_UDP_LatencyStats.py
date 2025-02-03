import matplotlib.pyplot as plt
import numpy as np

# Define X-axis labels 
resolutions = ("Raspberry Pi To Laptop ", "Laptop To Raspberry Pi", "Raspberry Pi To Display Image")
# TCP Latency Data
tcp_latency = {
    'Median': (11.835, 4.078, 6.724),
    'Standard Deviation': (5.227, 5.212, 14.332),
    'Variance': (3.372, 4.927, 2.937),
}

# UDP Latency Data
udp_latency = {
    'Median': (9.559, 2.463, 2.791),
    'Standard Deviation': (2.911, 0.834, 1.189),
    'Variance': (3.340, 0.193, 0.881),
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

axs[0].set_title("TCP Statistical Data at 640x480")
axs[0].set_ylabel("Latency (ms)")
axs[0].set_xticks(x + width, resolutions)
axs[0].legend(loc="upper left")
axs[0].set_ylim(0, max(max(tcp_latency.values())) + 4)

# Plot UDP data (Right subplot)
for i, (category, values) in enumerate(udp_latency.items()):
    offset = width * i  
    rects = axs[1].bar(x + offset, values, width, label=category, color=colors[i])
    axs[1].bar_label(rects, padding=2)

axs[1].set_title("UDP Statistical Data at 640x480")
axs[1].set_ylabel("Latency (ms)")
axs[1].set_xticks(x + width, resolutions)
axs[1].legend(loc="upper left")
axs[1].set_ylim(0, max(max(udp_latency.values())) + 5)

plt.savefig("TCP_UDP_StatData.png")

# Show the plot
plt.show()
