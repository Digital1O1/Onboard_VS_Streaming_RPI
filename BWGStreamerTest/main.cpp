#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>

using namespace cv;

std::mutex irMutex;
std::mutex visibleMutex;

#define ESC_KEY 27
#define HORIZONTAL_RESOLUTION 640
#define VERTICAL_RESOLUTION 480

void captureVisibleFrames(VideoCapture &cap, Mat *frame, const std::string &windowName)
{
    while (true)
    {
        // auto captureVisibleFrameSTART = std::chrono::high_resolution_clock::now();
        visibleMutex.lock();
        cap >> *frame;
        visibleMutex.unlock();

        if (frame->empty())
        {
            std::cerr << "Error: Couldn't read frame from camera" << std::endl;
            break;
        }
        // cv::imshow("visibleFrames", frame);
    }
}
void captureIRFrames(VideoCapture &cap, Mat *frame, const std::string &windowName)
{
    cv::Mat flipped;
    while (true)
    {
        irMutex.lock();
        cap >> *frame;
        cv::flip(*frame, *frame, 1);
        irMutex.unlock();

        // cv::flip(*frame, *frame, 1);
        if (frame->empty())
        {
            std::cerr << "Error: Couldn't read frame from camera" << std::endl;
            break;
        }
        // cv::imshow("irFrames", frame);
    }
}

int main()
{

    // std::string pipeline = R"(
    //     libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" !
    //     video/x-raw,width=1280,height=720,framerate=30/1 !
    //     videoconvert !
    //     v4l2h264enc extra-controls="controls,repeat_sequence_header=1" !
    //     video/x-h264,profile=baseline !
    //     appsink
    // )";

    // R"()" used to create a raw string literal
    std::string visibleCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! 
    video/x-raw,width=640,height=480,framerate=30/1 ! 
    videoconvert ! 
    video/x-raw,format=(string)BGR ! 
    queue ! 
    appsink
)";

    std::string irCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! 
    video/x-raw,width=640,height=480,framerate=30/1 ! 
    videoconvert ! 
    video/x-raw,format=(string)BGR ! 
    queue ! 
    appsink
)";

    cv::VideoCapture capIR(visibleCameraPipeline, cv::CAP_GSTREAMER);
    cv::VideoCapture visibleCap(irCameraPipeline, cv::CAP_GSTREAMER);

    // Create seperate thread to capture incoming frames
    // void captureIRFrames(VideoCapture &cap, Mat *frame, const std::string &windowName)

    // This has to be done since the image acqusition is being done on seperate threads
    Mat visibleFrames, irFrames;
    Mat *irFramePtr = &irFrames;
    Mat *visibleFramePtr = &visibleFrames;
    irFrames = Mat::zeros(Size(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), CV_8UC3);
    visibleFrames = Mat::zeros(Size(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), CV_8UC3);

    std::thread captureIRThread(captureIRFrames, std::ref(capIR), irFramePtr, "IR Camera");
    std::thread captureVisibleThread(captureVisibleFrames, std::ref(visibleCap), visibleFramePtr, "Visible Camera");
    // Check if the cameras are opened
    if (!capIR.isOpened() || !visibleCap.isOpened())
    {
        std::cerr << "Error: Cameras not accessible" << std::endl;
        return -1;
    }
    else
    {
        printf("\nGSTREAMER PIPELINES OPENED \r\n");
    }
    while (true)
    {
        // Mat visibleFrames, irFrames;
        visibleCap >> visibleFrames;
        capIR >> irFrames;
        // cv::flip(irFrames, irFrames, 1);

        // Check if any of the frames is empty
        if (visibleFrames.empty() || irFrames.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        cv::imshow("visibleFrames", visibleFrames);
        cv::imshow("irFrames", irFrames);

        if (cv::waitKey(1) == ESC_KEY) //  Pres ESC key to exit program
        {
            std::cout << "Exiting...\n";
            break;
        }
    }
    captureIRThread.join();
    captureVisibleThread.join();
    capIR.release();
    visibleCap.release();
    destroyAllWindows();
    return 0;
}