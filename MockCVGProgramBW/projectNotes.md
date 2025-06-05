# Libraries used

## To enable camera stuff with RPICM4
- Reference link : [here](https://www.waveshare.com/wiki/CM4-IO-BASE-A)
- Don't worry about the config file for the CSI/DSI stuff

## WiringPi
```bash
#!/bin/bash

sudo apt update
sudo apt install git build-essential

git clone https://github.com/WiringPi/WiringPi.git ~/WiringPi
cd ~/WiringPi
./build

# To test if install was successful
#VERSION=$(gpio -v)
echo "WiringPi version info: $(gpio -v)"
```

## GStreamer
```bash
sudo apt install -y gstreamer1.0-libcamera

sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
```
# How to check

## Camera 'names' for GStreamer pipeilnes
```bash
libcamera-hello --list-cameras
```

## Element capabilities
```bash
gst-inspect-1.0 libcamerasrc
```

## Which videoconert formats are supported 
```bash
gst-inspect-1.0 videoconvert | grep format
gst-inspect-1.0 videoconvert
# Example output
format: { (string)ABGR64_LE, (string)BGRA64_LE, (string)AYUV64, (string)ARGB64_LE, (string)ARGB64, (string)RGBA64_LE, (string)ABGR64_BE, (string)BGRA64_BE, (string)ARGB64_BE, (string)RGBA64_BE, (string)GBRA_12LE, (string)GBRA_12BE, (string)Y412_LE, (string)Y412_BE, (string)A444_10LE, (string)GBRA_10LE, (string)A444_10BE, (string)GBRA_10BE, (string)A422_10LE, (string)A422_10BE, (string)A420_10LE, (string)A420_10BE, (string)RGB10A2_LE, (string)BGR10A2_LE, (string)Y410, (string)GBRA, (string)ABGR, (string)VUYA, (string)BGRA, (string)AYUV, (string)ARGB, (string)RGBA, (string)A420, (string)AV12, (string)Y444_16LE, (string)Y444_16BE, (string)v216, (string)P016_LE, (string)P016_BE, (string)Y444_12LE, (string)GBR_12LE, (string)Y444_12BE, (string)GBR_12BE, (string)I422_12LE, (string)I422_12BE, (string)Y212_LE, (string)Y212_BE, (string)I420_12LE, (string)I420_12BE, (string)P012_LE, (string)P012_BE, (string)Y444_10LE, (string)GBR_10LE, (string)Y444_10BE, (string)GBR_10BE, (string)r210, (string)I422_10LE, (string)I422_10BE, (string)NV16_10LE32, (string)Y210, (string)v210, (string)UYVP, (string)I420_10LE, (string)I420_10BE, (string)P010_10LE, (string)NV12_10LE32, (string)NV12_10LE40, (string)P010_10BE, (string)NV12_10BE_8L128, (string)Y444, (string)RGBP, (string)GBR, (string)BGRP, (string)NV24, (string)xBGR, (string)BGRx, (string)xRGB, (string)RGBx, (string)BGR, (string)IYU2, (string)v308, (string)RGB, (string)Y42B, (string)NV61, (string)NV16, (string)VYUY, (string)UYVY, (string)YVYU, (string)YUY2, (string)I420, (string)YV12, (string)NV21, (string)NV12, (string)NV12_8L128, (string)NV12_64Z32, (string)NV12_4L4, (string)NV12_32L32, (string)NV12_16L32S, (string)Y41B, (string)IYU1, (string)YVU9, (string)YUV9, (string)RGB16, (string)BGR16, (string)RGB15, (string)BGR15, (string)RGB8P, (string)GRAY16_LE, (string)GRAY16_BE, (string)GRAY10_LE32, (string)GRAY8 }

```
## If a pipeline is working
```bash
gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
video/x-raw,format=BGR,width=640,height=480 ! videoconvert ! autovideosink
```
