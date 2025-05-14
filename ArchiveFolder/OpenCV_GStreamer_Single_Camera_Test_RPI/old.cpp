#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    printf("\n\n--------------------------------- PROGRAM STARTED --------------------------------- \r\n");

    // Construct GStreamer pipeline for camera capture
    // Old pipeline
    // String pipeline = "raspivid -t 0 -hf -fps 20 -w 300 -h 300 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=172.17.141.124 port=5000";

    // pipeline to send to Linux laptop
    String pipeline = "v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480,framerate=30/1 ! videoconvert ! jpegenc !  rtpjpegpay !  udpsink host=192.168.178.139 port=5200";

    // Pipieline used to recieve from Linux laptop
    std::string sendToPiPipeline = "appsrc ! videoconvert ! queue ! vaapih264enc ! h264parse !  rtph264pay ! udpsink host=172.17.141.124 port=5001";

    // Capture frames on the RPI
    VideoCapture sendToLinuxLaptop(pipeline, CAP_GSTREAMER);
    // Used to send captured frames to Linux Laptop
    cv::VideoWriter RPIWriter(pipeline, cv::VideoWriter::fourcc('H', '2', '6', '4'), 25, cv::Size(640, 480), true);

    if (!sendToLinuxLaptop.isOpened())
    {
        cerr << "Error: Unable to open camera." << endl;
        return -1;
    }

    Mat imx219Frame;

    while (true)
    {
        sendToLinuxLaptop >> imx219Frame; // Capture imx219Frame

        if (imx219Frame.empty())
        {
            cerr << "Error: Blank imx219Frame captured" << endl;
            break;
        }

        imshow("Sending to Linux laptop", imx219Frame);

        if (waitKey(1) == 27) // Exit when Esc key is pressed
            break;
    }

    sendToLinuxLaptop.release();
    destroyAllWindows();

    return 0;
}
