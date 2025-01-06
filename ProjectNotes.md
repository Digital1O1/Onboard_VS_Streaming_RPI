<!-- # Ian's GStreamer stuff
## Server

`host=<ip address of viewing pi>`

Maybe this, but it doesn't always work: `$(arp -n | grep e4.5f.01.df.c5.8f | awk '{print $1}')`


### libcamerasrc
```sh
gst-launch-1.0 libcamerasrc auto-focus-mode=AfModeContinuous ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! queue ! udpsink host=172.17.140.124 port=5000
```

### rpicam-vid | Bookworm
```sh
rpicam-vid -t 0 --width 640 --height 480 --framerate 24 -n --inline -o - | gst-launch-1.0 fdsrc fd=0 ! h264parse ! h264timestamper ! rtph264pay ! udpsink host=$(arp -n | grep e4.5f.01.df.c5.8f | awk '{print $1}') port=5000
```
    VideoWriter write(sendToPiPipeline, cv::CAP_GSTREAMER, 0,25, 640, 480);

## Reciever

```sh
gst-launch-1.0 udpsrc address=172.17.140.124  port=5000 caps=application/x-rtp ! rtpjitterbuffer latency=100 drop-on-latency=true ! rtph264depay ! h264parse ! v4l2h264dec ! queue ! glimagesink sync=false
```

---

<br> -->
<!-- 

# My stuff

# Linux Laptop
### Pipeline based from [this reference](https://raspberrypi.stackexchange.com/questions/27082/how-to-stream-raspivid-to-linux-and-osx-using-gstreamer-vlc-or-netcat) THAT WORKS

```sh
gst-launch-1.0 -v tcpclientsrc host=172.17.141.124 port=5000  ! gdpdepay !  rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=false
```

### GPT response WORKS
```sh
gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp, encoding-name=H264,payload=96 ! rtph264depay ! decodebin ! videoconvert ! autovideosink sync=false
```



# Raspberry PI Buster

## Use this for testing purposes only
```sh
gst-launch-1.0 v4l2src device=/dev/video0 ! video/x-raw, width=1280, height=720, framerate=30/1 ! videoconvert ! videoscale ! clockoverlay time-format="%D %H:%M:%S" ! video/x-raw, width=640, height=360 ! autovideosink
```
### Pipeline based from [this reference](https://raspberrypi.stackexchange.com/questions/27082/how-to-stream-raspivid-to-linux-and-osx-using-gstreamer-vlc-or-netcat) THAT WORKS

```sh
 raspivid -t 0 -hf -fps 20 -w 300 -h 300 -o - | gst-launch-1.0 fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=172.17.141.124 port=5000
```

### GPT response THAT WORKS
```sh
raspivid -t 0 -w 640 -h 480 -fps 25 -b 2000000 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000
```

## Raspicam
```sh
# Testing purposes
gst-launch-1.0 v4l2src device=/dev/video0 ! video/x-raw, width=1280, height=720, framerate=30/1 ! videoconvert ! videoscale ! clockoverlay time-format="%D %H:%M:%S" ! video/x-raw, width=640, height=360 ! autovideosink
```
```sh
gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 ! rtpjpegdepay ! jpegdec ! videoconvert ! fpsdisplaysink video-sink=autovideosink sync=0 silent=true
```

### Reference link for streaming : https://raspberrypi.stackexchange.com/questions/27082/how-to-stream-raspivid-to-linux-and-osx-using-gstreamer-vlc-or-netcat


# Latency testing

## Reference link [here](https://gstreamer.freedesktop.org/documentation/additional/design/tracing.html?gi-language=c#try-it)

## V4L2 

```sh
#TEST THIS OUT
gst-launch-1.0 v4l2src ! capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2h264enc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000

# THIS PRINTS OUT THE TIME
# Moving forward must use v4l2
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency(flags=element+pipeline)" GST_DEBUG_FILE=./rpiVisibleCamera.log gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,width=640,height=480,framerate=30/1,format=NV12 ! v4l2h264enc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000
```

## V4L2 stuff to test out

### Adding timestamps everytime the log file is saved

```bash
# Generate a unique log file name with timestamp
LOG_FILE="./rpiVisibleCamera_$(date +"%Y-%m-%d_%H:%M:%S").log"

# Run GStreamer pipeline with unique log file name
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency(flags=element+pipeline)" GST_DEBUG_FILE="$LOG_FILE" gst-launch-1.0 v4l2src device=/dev/video1 ! video/x-raw,width=640,height=480,framerate=30/1,format=NV12 ! v4l2h264enc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000

```
 -->


~~# Latency test stuff not sure if this is needed~~

<!-- ```bash
# 'Snow' GST_DEBUG testing program
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency(flags=element+pipeline)" GST_DEBUG_FILE=./latency.log gst-launch-1.0 -v videotestsrc pattern=snow ! video/x-raw,width=640,height=480,framerate=30/1 ! videoconvert ! autovideosink
```


```bash
# Currently testing
# Update 4/25 : latency with raspivid command doesn't work, must use v4l2
GST_DEBUG="GST_TRACER:7" GST_TRACERS="latency(flags=element+pipeline)" GST_DEBUG_FILE=./rpiLatency.log gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000  | raspivid -n -t 0 -w 640 -h 480 -fps 25 -b 2000000 -o - -cs 1 | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000
``` -->
<!-- 

# Gstreamer breakdown of how things work

# Pipeline parameters for RPI Buster: 

## With preview 

```bash
raspivid -t 0 -w 640 -h 480 -fps 25 -b 2000000 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000
```
## No preview : 
```
raspivid -n -t 0 -w 640 -h 480 -fps 25 -b 2000000 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.141.174 port=5000
```
`raspivid`
- t 0 : Specifies that the camera should run indefinitely until stopped.
- w 640 -h 480: Sets the width and height of the video to 640x480 pixels.
- fps 25: Sets the frame rate to 25 frames per second.
- b 2000000: Sets the bitrate to 2Mbps.
- o -: Specifies that the output should be written to standard output (stdout).

`- | (pipe)` 
- Sends the output of raspivid (video stream) to the next command via a pipe.

`gst-launch-1.0 -v` 
- `-v --verbose`
  - Output status information and property notifications

`fdsrc`
- Read data from unix file descriptor
- To generate data
  - Enter data in console then press ENTER
  - The pipeline SHOULD dump the data packets to the console
- There's a `timeout` property, but it's probably not important to the streaming project
- Reference link : https://gstreamer.freedesktop.org/documentation/coreelements/fdsrc.html?gi-language=c

`h264parse`
- Parses H.264 streams
`fdsrc` 
- Reads data from a file descriptor, in this case, the video stream from raspivid.

`h264parse` 
- Parses the H.264 stream.

`rtph264pay` 
- Encapsulates H.264 packets into RTP packets. 
- Options:
    - `config-interval=10` 
      - Specifies that SPS and PPS should be sent every 10 frames.
    - `pt=96` 
      - Specifies the payload type for the RTP packets.
      - RTP packets : Real-time transport protocol

`udpsink` 
- Sends data over UDP to a specified IP address and port. Options:

`host=YOUR_LINUX_MACHINE_IP_ADDRESS` 
- Specifies the IP address of the Linux machine.

`port=5000` 
- Specifies the UDP port number.

---

# Pipeline for Linux machine

```bash
gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp, encoding-name=H264,payload=96 ! rtph264depay ! decodebin ! videoconvert ! autovideosink sync=false
```
## Breakdown of Linux machine pipeline
`gst-launch-1.0` 
- `udpsrc`: 
  - Receives data over UDP from a specified port.
- `application/x-rtp, encoding-name=H264, payload=96`: 
  - Specifies that the incoming data is RTP packets containing H.264-encoded video. The payload type is set to 96, matching the sender's configuration.
- `rtph264depay`
  - : Depayloads the H.264 RTP packets.
- `decodebin`: 
  - Automatically selects and configures a suitable decoder based on the incoming data.
- `videoconvert` 
  - Converts the video from one pixel format to another if necessary.
- `autovideosink sync=false`: 
  - Automatically selects and configures a suitable video sink (display) for the video stream. 
  - `sync=false` is used to disable synchronization with the clock, which can help reduce latency in the display of the video.


<br>

---

<br> -->

# Current goals with streaming project
- [ ] Determine latency with both UDP and TCP/IP when using GStreamer
- [ ] Learn how to use TMUX in GStreamer

- [x] Figure out pipeline to send YUV420 frames over from RPI to laptop 
  - [x]  Record video UP to 5 mins for now on laptop (Done with Picamera2)
- [ ] 'Split' the composite video into different channels on laptop

# Feedback from manuscript

## First reviewer

#### Major comments
- Section 2.1
  -   ~~Helpful to have labeled figure that shows all components on the sensor~~ 
  -   ~~Arrows pointing to each component would be nice~~ 
  -   ~~Also nice to have diagram showing processing flow for data~~ 
      -   Where data goes and which libraries/devices do what 
- Provide figure showing setup for latency measurements in section 2.2 
  -  Could be simple picture of the setup
- State the required image resolution for practical use of the goggles
  - Since we suggested to use lower resolution images to reduce the latency the reviewer request to quantify if lowering the resolution will have on the overall performance of the surgeon
  - Redo the sorting task and toggle hte image resolution 
- Has other wireless transmission methods been explored?
  - Have you tired image compression?
- Add figure comparing UDP and TCP/IP transmission speed
- Provide 'back of the envvelope' calculations on the data rate needed for the CVG given the size of a single image from each camera and the desired frame rate
  - Doing so will help put the results into context 
  - Also state total number of pixels and bit dpeth for each camera in Section 2.1
- Was any experiments ran by reducing the frame rate in addition to reducing the image size?

#### Minor Comments

1. **Define CWL**  
   - Please provide a definition or explanation for CWL.

2. **Clarify terminology in Section 2.3**  
   -    In Section 2.3, the phrase "polyurethane tissue-mimicking optical properties" is unclear. Do you mean "phantom" instead of "properties"?

3. **Explain hardware selection**  
   -    Could you add an explanation of why the specific computer hardware was chosen for the experiments? What were the selection criteria—cost, availability, or something else?

4. **Correct table labeling and references**  
   -    There are two figures labeled as "Table 1." Please correct these labels and verify all references to the tables in the text for accuracy.

5. **Visualize latency testing results**  
   -    Can the latency testing results (currently presented in the third table) be shown as a bar plot for better visualization?

6. **Comment on asymmetric data transfer speeds**  
   Why are the data transfer speeds asymmetric? Specifically, why is the transfer from the laptop to the Raspberry Pi faster than the reverse?

7. **Relevance of fluorescence fragment size**  
   -    Please comment on the significance of the fluorescence fragment sizes compared to the dimensions of objects the surgeon would be resecting.

8. **Specify WiFi band**  
   -    What WiFi band are you using for the experiments—2.4 GHz or 5 GHz?

9. **Resolution of sorting task images**  
   -    What resolution were the images provided to participants in the sorting task?


## Second reviewer

### Suggested Improvements

#### Edge Computing Justification  
The lightweight nature of image overlay computations raises doubts about the need for fog computing. 

Demonstrating the limitations of edge processing would strengthen the argument.

#### UDP Performance Data  
Since the sorting task relies on UDP, a comprehensive analysis of UDP performance, including its addition to Table 2, is recommended.

#### Hardware Configuration Clarity  
Using two different laptops for TCP and UDP complicates the analysis, making it unclear whether differences arise from protocol or hardware variations.

#### Technical Data Presentation  
<!-- The table labeled as "Table 1" appears to be Table 3. It shows an unexplained four-fold difference in data transfer duration between Raspberry Pi-to-computer and computer-to-Raspberry Pi communications. Additionally, the reported transfer rates are significantly lower than the wireless speeds specified in Table 1, suggesting possible implementation constraints rather than hardware limitations. These discrepancies warrant discussion.   -->

The mislabeled table shows a significant, unexplained difference in data transfer speeds between directions and discrepancies with expected wireless speeds, suggesting possible implementation constraints.



#### Sorting Task Analysis  
While the sorting task provides interesting insights, it could merit a dedicated experimental investigation evaluating performance across various simulated latency conditions. The scope should extend beyond basic sorting performance to include user experience factors, similar to research on display lag discomfort (see, for example, [https://doi.org/10.3389/frvir.2020.582204](https://doi.org/10.3389/frvir.2020.582204)).  
**CAUTION:** This email originated from outside UTSW. Please be cautious of links or attachments and validate the sender's email address before replying.  

<!-- 

# Debugging tools

## Printing debugging information

### The debug log

Debug output controlled with `GST_DEBUG` enviornment variable
- Below is an example with `GST_DEBUG=2`
  - Keep in mind that debug log can be verbose, so much so that it 
    - Can crash applications
    - Fill up megabytes of text code
    - Because of these reasons are why the logs are categorized and you seldomly need to enable all categories at once
```bash
# GST_DEBUG=2

| # | Name    | Description                                                    |
|---|---------|----------------------------------------------------------------|
| 0 | none    | No debug information is output.                                |
| 1 | ERROR   | Logs all fatal errors. These are errors that do not allow the  |
|   |         | core or elements to perform the requested action. The          |
|   |         | application can still recover if programmed to handle the      |
|   |         | conditions that triggered the error.                           |
| 2 | WARNING | Logs all warnings. Typically these are non-fatal, but          |
|   |         | user-visible problems are expected to happen.                  |
| 3 | FIXME   | Logs all "fixme" messages. Those typically that a codepath that|
|   |         | is known to be incomplete has been triggered. It may work in   |
|   |         | most cases, but may cause problems in specific instances.      |
| 4 | INFO    | Logs all informational messages. These are typically used for  |
|   |         | events in the system that only happen once, or are important   |
|   |         | and rare enough to be logged at this level.                    |
| 5 | DEBUG   | Logs all debug messages. These are general debug messages for  |
|   |         | events that happen only a limited number of times during an    |
|   |         | object's lifetime; these include setup, teardown, change of    |
|   |         | parameters, etc.                                               |
| 6 | LOG     | Logs all log messages. These are messages for events that      |
|   |         | happen repeatedly during an object's lifetime; these include   |
|   |         | streaming and steady-state conditions. This is used for log    |
|   |         | messages that happen on every buffer in an element for example.|
| 7 | TRACE   | Logs all trace messages. Those are message that happen very    |
|   |         | very often. This is for example is each time the reference     |
|   |         | count of a GstMiniObject, such as a GstBuffer or GstEvent, is  |
|   |         | modified.                                                      |
| 9 | MEMDUMP | Logs all memory dump messages. This is the heaviest logging and|
|   |         | may include dumping the content of blocks of memory.           |
+------------------------------------------------------------------------------+
```

To enable debug output
- Set `GST_DEBUG` enviornment variable to `desired debug level`
- All levels below the set level will be displayed
- Each `plugin` or `part of the GStreamer` defines it's own category
  - Lets you specify debug level for each individual category 
    - Example : `GST_DEBUG=2,audiotestsrc:6`
    - Uses debug level 6 for audiotestsrc
    - Uses debug level 2 for everything else

### The `GST_DEBUG` environment variable 

A comma-seperated lists of category level pairs 
- Have an optional level at the beginning that represents default debug level for all categories
- The `*` wildcard is also available 
  - Example : `GST_DEBUG=2,audio*:6`
    - Uses debug level 6 for all categories starting with the word `audio`
    - Note : `GST_DEBUG=*:2` == `GST_DEBUG=2`

`gst-launch-1.0 --gst-debug-help` 
- Use to get list of all registered categories 
- Keep in mind
  - Each plugin register has it's own categories
  - Why is this important?
    - Installing/removing plugins will change the list

## To determine supported video formats 
```bash
gst-device-monitor-1.0 Video
```


<br>

---

<br>
 -->
