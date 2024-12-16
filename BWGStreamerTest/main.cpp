#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>

using namespace cv;

std::mutex irMutex, visibleMutex;
std::atomic<bool> captureFrames(true);

#define ESC_KEY 27
#define HORIZONTAL_RESOLUTION 640
#define VERTICAL_RESOLUTION 480

/*
    Why use tempFrames
        - Prevent partical updates
            - Functions like cv::flip could involve alot of intermediate steps that update the Mat object
            - Functions like
                - Memory allocation
                - Resizing
                - Ect....
            - If another thread tries to read the shared frame during the updates it could corrput or cause an incompletion of the data
        - Reduce mutex lock times
            - Using a temporary variable to read and process a frame allows you to only lock the mutex when ASSIGNING THE FULLY PROCESSED FRAME TO THE SHARED VARIABLE
            - Thus minimizes the time spent holding the mutex
            - And allows other threads to access the frame more frequently
        - Isolating the processing logic
            - Using tempFrame helps ensure other opeartions like flipping/resizing are INDEPENDENT of the shared frame
            - Thus preventing unintended side effects caused by direct writes to the shared object
*/
void captureVisibleFrames(VideoCapture &cap, Mat &frame)
{
    while (captureFrames)
    {
        Mat tempFrame; // See commented section above for the logic behind using a temporary Mat object
        cap >> tempFrame;

        if (tempFrame.empty())
        {
            std::cerr << "Error: Couldn't read frame from visible camera" << std::endl;
            captureFrames = false;
            break;
        }

        // Write frame directly with lock
        std::lock_guard<std::mutex> lock(visibleMutex);
        frame = tempFrame; // Assign directly without cloning
    }
}

void captureIRFrames(VideoCapture &cap, Mat &frame)
{
    while (captureFrames)
    {
        Mat tempFrame;
        cap >> tempFrame;

        if (tempFrame.empty())
        {
            std::cerr << "Error: Couldn't read frame from IR camera" << std::endl;
            captureFrames = false;
            break;
        }

        cv::flip(tempFrame, tempFrame, 1);

        // Write frame directly with lock
        std::lock_guard<std::mutex> lock(irMutex);
        frame = tempFrame; // Assign directly without cloning
    }
}

int main()
{
    // To determine the other video/x-raw parameters you can use : gst-device-monitor-1.0
    std::string visibleCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! 
    video/x-bayer,width=640,height=480,framerate=30/1 ! 
    videoconvert ! 
    video/x-raw,format=(string)BGR ! 
    queue ! 
    appsink
)";

    std::string irCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! 
    video/x-bayer,width=640,height=480,framerate=30/1 ! 
    videoconvert ! 
    video/x-raw,format=(string)BGR ! 
    queue ! 
    appsink
)";

    // std::string visibleCameraPipeline = R"(
    //     libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" !
    //     video/x-bayer,format=(string)rggb10,width=640,height=480,framerate=30/1 !
    //     queue !
    //     appsink
    // )";

    // std::string sendForProcessing = R"(
    //     appsrc ! videoconvert !
    //     v4l2h264enc extra-controls="controls,repeat_sequence_header=1" !
    //     h264parse ! rtph264pay !
    //     udpsink host=172.17.140.56 port=5001
    // )";

    std::string sendForProcessing = R"(
    appsrc ! videoconvert ! video/x-raw,format=BGR ! 
    v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 
    h264parse ! rtph264pay ! 
    udpsink host=172.17.140.56 port=5001
)";

    VideoCapture capIR(irCameraPipeline, cv::CAP_GSTREAMER);
    VideoCapture capVisible(visibleCameraPipeline, cv::CAP_GSTREAMER);

    // VideoWriter writer(sendForProcessing, cv::CAP_GSTREAMER, 0, 30, Size(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), true);

    VideoWriter writer(sendForProcessing, cv::CAP_GSTREAMER, 0, 30, Size(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), true);

    if (!writer.isOpened())
    {
        std::cerr << "Error: Couldn't open video writer pipeline" << std::endl;
        return -1;
    }
    if (!capIR.isOpened() || !capVisible.isOpened())
    {
        std::cerr << "Error: Cameras not accessible" << std::endl;
        return -1;
    }

    Mat visibleFrame = Mat::zeros(Size(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), CV_8UC3);
    Mat irFrame = Mat::zeros(Size(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION), CV_8UC3);

    std::thread captureIRThread(captureIRFrames, std::ref(capIR), std::ref(irFrame));
    std::thread captureVisibleThread(captureVisibleFrames, std::ref(capVisible), std::ref(visibleFrame));
    // std::thread captureThread(captureFramesAndSend, std::ref(cap), std::ref(writer));

    Mat localVisibleFrame, localIRFrame, sideBySide;

    while (captureFrames)
    {
        // Mat localVisibleFrame, localIRFrame, sideBySide;

        // Safely read frames under lock via using scoping blocks (The curly braces) to limit the 'lifetime' of each std::lock_guard object since it gets destoryed when the block ends, thus releases the assocaited mutex
        {
            std::lock_guard<std::mutex> lockVisible(visibleMutex);
            localVisibleFrame = visibleFrame; // Access directly without cloning
        }

        {
            std::lock_guard<std::mutex> lockIR(irMutex);
            localIRFrame = irFrame; // Access directly without cloning
        }

        cv::hconcat(localVisibleFrame, localIRFrame, sideBySide);

        // if (!localVisibleFrame.empty())
        // {
        //     writer.write(localVisibleFrame); // Write the BGR frame directly
        // }

        if (!sideBySide.empty())
            imshow("Visible frame | | IR frame", sideBySide);

        if (cv::waitKey(1) == ESC_KEY)
        {
            captureFrames = false;
            break;
        }
    }

    captureIRThread.join();
    captureVisibleThread.join();

    capIR.release();
    capVisible.release();
    destroyAllWindows();

    return 0;
}
