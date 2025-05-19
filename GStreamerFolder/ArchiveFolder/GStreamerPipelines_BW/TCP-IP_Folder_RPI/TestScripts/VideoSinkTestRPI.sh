gst-launch-1.0 -vvv videotestsrc is-live=true ! openh264enc ! rtph264pay ! rtpstreampay \
    ! tcpserversink port=5000


#gst-launch-1.0 -vvv videotestsrc is-live=true ! tcpserversink host=0.0.0.0 port=7001
