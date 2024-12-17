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
#define RESOLUTION_WIDTH 640
#define RESOLUTION_HEIGHT 480
#define YUV420_WIDTH_RESOLUTION 640
#define YUV420_HEIGHT_RESOLUTION 720

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
        // cv::Mat tempFrameBGR;
        // cv::cvtColor(tempFrame, tempFrameBGR, cv::COLOR_YUV2BGR_I420);
        // cv::imshow("tempFrame", tempFrameBGR);
        // cv::waitKey(1);

        // Write frame directly with lock
        std::lock_guard<std::mutex> lock(visibleMutex);
        frame = tempFrame; // Assign directly without cloning
        // frame = tempFrameBGR;
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
        // cv::Mat tempFrameBGR;
        // cv::cvtColor(tempFrame, tempFrameBGR, cv::COLOR_YUV2BGR_I420);

        // Write frame directly with lock
        std::lock_guard<std::mutex> lock(irMutex);
        frame = tempFrame; // Assign directly without cloning
        // frame = tempFrameBGR;
    }
}

int main()
{
    // To determine the other video/x-raw parameters you can use : gst-device-monitor-1.0
    std::string visibleCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! 
    video/x-raw,format=(string)I420,width=640,height=480,framerate=30/1 ! 
    queue ! 
    appsink
)";

    std::string irCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! 
    video/x-raw,format=(string)I420,width=640,height=480,framerate=30/1 ! 
    queue ! 
    appsink
)";

    // DOESN'T WORK
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

    //     std::string sendForProcessing = R"(
    //     appsrc  ! v4l2h264enc extra-controls="controls,repeat_sequence_header=1" !
    //     h264parse ! rtph264pay !
    //     udpsink host=172.17.140.56 port=5001
    // )";

    std::string sendForProcessing = R"(
appsrc ! video/x-raw,format=I420,width=640,height=480,framerate=30/1 ! 
v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! 
h264parse ! rtph264pay ! 
udpsink host=172.17.140.56 port=5001
)";

    // Capture YUV420 frames
    VideoCapture capIR(irCameraPipeline, cv::CAP_GSTREAMER);
    VideoCapture capVisible(visibleCameraPipeline, cv::CAP_GSTREAMER);

    // Send YUV420 frames to linux laptop
    VideoWriter sendToLinuxLaptopWriter(sendForProcessing, cv::CAP_GSTREAMER, 0, 30, Size(640, 480), true);

    if (!capIR.isOpened() || !capVisible.isOpened() || !sendToLinuxLaptopWriter.isOpened())
    {
        std::cerr << "Error: Cameras not accessible" << std::endl;
        return -1;
    }

    Mat visibleFrame = Mat::zeros(Size(RESOLUTION_WIDTH, RESOLUTION_HEIGHT), CV_8UC3);
    Mat irFrame = Mat::zeros(Size(RESOLUTION_WIDTH, RESOLUTION_HEIGHT), CV_8UC3);

    std::thread captureIRThread(captureIRFrames, std::ref(capIR), std::ref(irFrame));
    std::thread captureVisibleThread(captureVisibleFrames, std::ref(capVisible), std::ref(visibleFrame));
    // std::thread captureThread(captureFramesAndSend, std::ref(cap), std::ref(sendToLinuxLaptopWriter));

    Mat localVisibleFrame, localIRFrame, sideBySide, frameBGR0, frameBGR1;

    while (captureFrames)
    {
        // Mat localVisibleFrame, localIRFrame, sideBySide;

        // Safely read frames under lock via using scoping blocks (The curly braces) to limit the 'lifetime' of each std::lock_guard object since it gets destoryed when the block ends, thus releases the assocaited mutex
        {
            std::lock_guard<std::mutex> lockVisible(visibleMutex);
            localVisibleFrame = visibleFrame;
        }

        {
            // std::lock_guard<std::mutex> lockIR(irMutex);
            // localIRFrame = irFrame;

            // Mat yuvFrame;
            // cv::cvtColor(irFrame, yuvFrame, cv::COLOR_BGR2YUV_I420);

            std::lock_guard<std::mutex> lockIR(irMutex);
            localIRFrame = irFrame;
        }

        // cv::Size size = localIRFrame.size();
        // int width = size.width;
        // int height = size.height;
        // std::cout << "Width: " << width << ", Height: " << height << " " << "Type : " << localIRFrame.type() << std::endl;
        // std::cout << "localIRFrame Row : " << localIRFrame.rows;
        //  cv::imshow("localVisibleFrame", localVisibleFrame);
        // cv::imshow("localIRFrame", localIRFrame);

        // cv::hconcat(localVisibleFrame, localIRFrame, sideBySide);

        if (!localIRFrame.empty())
        {
            // Create an empty YUV420 frame
            int width = localIRFrame.cols;
            int height = localIRFrame.rows;

            // YUV420 frame requires the Y, U, and V channels
            cv::Mat yuv420_img(height + height / 2, width, CV_8UC1); // Total size for YUV420 (I420)
                                                                     // cv::imshow("yuv420_img", yuv420_img);
            //  Set the Y channel to the grayscale image (same intensity)
            //  localIRFrame.copyTo(yuv420_img(cv::Rect(0, 0, width, height)));
            yuv420_img(cv::Rect(0, 0, width, height)) = localIRFrame;

            // U and V channels (subsampled)
            // Set both U and V to 128 (neutral value)
            cv::Mat u_channel(height / 2, width / 2, CV_8UC1, cv::Scalar(128));
            cv::Mat v_channel(height / 2, width / 2, CV_8UC1, cv::Scalar(128));

            // Assign U and V channels using the assignment operator
            yuv420_img(cv::Rect(0, height, width / 2, height / 2)) = u_channel;
            yuv420_img(cv::Rect(width / 2, height, width / 2, height / 2)) = v_channel;
            cv::imshow("localIRFrame", localIRFrame);
            // Send the converted frame
            // sendToLinuxLaptopWriter.write(yuv_img);
        }

        // if (!sideBySide.empty())
        //     imshow("Visible frame | | IR frame", sideBySide);
        // cv::imshow("localIRFrame", localIRFrame);
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
