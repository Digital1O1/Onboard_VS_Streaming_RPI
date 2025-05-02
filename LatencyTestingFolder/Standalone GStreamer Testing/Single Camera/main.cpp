#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define HORIZONTAL_RESOLUTION 640
#define VERTICAL_RESOLUTION 480
#define FRAME_RATE 30

int main()
{
    printf("\n\n--------------------------------- PROGRAM STARTED --------------------------------- \r\n");

    // The sink caps for the 'rtpjpegdepay' need to match the src caps of the 'rtpjpegpay' of the sender pipeline
    // Added 'videoconvert' at the end to convert the images into proper format for appsink, without
    // 'videoconvert' the receiver will not read the frames, even though 'videoconvert' is not present
    // in the original working pipeline

    string raspividPipeline = "raspivid -t 0 -n -w 640 -h 480 -fps 60 -b 2000000 -o - -cs 1 | \
gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=10 pt=96 ! \
udpsink host=172.17.141.174 port=5000 ";

    VideoCapture cap(raspividPipeline, CAP_GSTREAMER);

    if (!cap.isOpened())
    {
        cerr << "VideoCapture not opened" << endl;
        exit(-1);
    }
    cv::Mat croppedVisibleFrames;
    while (true)
    {

        Mat visibleFrame;

        cap.read(visibleFrame);
        // cap >> visibleFrame;
        // imshow("receiver", visibleFrame);

        cv::Rect roiVisible(visibleFrame.cols / 4, visibleFrame.rows / 4, visibleFrame.cols / 2, visibleFrame.rows / 2);

        croppedVisibleFrames = visibleFrame(roiVisible);

        cv::resize(croppedVisibleFrames, croppedVisibleFrames, visibleFrame.size());

        imshow("From RPI", croppedVisibleFrames);

        if (waitKey(1) == 27)
        {
            break;
        }
    }

    return 0;
}