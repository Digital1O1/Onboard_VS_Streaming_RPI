#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

int main()
{
    String sendToLinuxLaptopPipeline = "appsrc ! v4l2h264enc extra-controls='controls, repeat_sequence_header=1' ! 'video/x-h264,level=(string)4.1,profile=(string)baseline' ! h264parse ! rtph264pay ! udpsink host=172.17.141.174 port=5000";
    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        cerr << "VideoCapture not opened" << endl;
        exit(-1);
    }

    VideoWriter writer(sendToLinuxLaptopPipeline, cv::CAP_GSTREAMER, 0, 30, Size(640, 480));

    if (!writer.isOpened())
    {
        cerr << "VideoWriter not opened" << endl;
        exit(-1);
    }

    while (true)
    {
        Mat frame;

        cap.read(frame);

        writer.write(frame);
    }

    return 0;
}