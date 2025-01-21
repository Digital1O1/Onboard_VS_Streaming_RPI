#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

int main()
{
    bool previewFlag = true;
    cv::Mat irFrames, visibleFrames, gray;

    // To determine camera name use : libcamera-hello --list
    // To determine supported color spaces for IMX219 use : gst-inspect-1.0 videoconvert

    // Use for RGB frames
    /*
    std::string visibleCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" !
    video/x-raw,width=640,height=480,framerate=30/1 !
    videoconvert !
    video/x-raw,format=(string)BGR !
    queue !
    appsink
)";

    std::string irCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" !
    video/x-raw,width=640,height=480,framerate=30/1 !
    videoconvert !
    video/x-raw,format=(string)BGR !
    queue !
    appsink
)";
    */

    // Use for YUV420 pipelines
    std::string visibleCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! 
    video/x-raw,width=640,height=480,framerate=30/1 ! 
    videoconvert ! 
    video/x-raw,format=(string)I420 ! 
    queue ! 
    appsink
)";

    std::string irCameraPipeline = R"(
    libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! 
    video/x-raw,width=640,height=480,framerate=30/1 ! 
    videoconvert ! 
    video/x-raw,format=(string)I420 ! 
    queue ! 
    appsink
)";

    cv::VideoCapture irCapture(visibleCameraPipeline, cv::CAP_GSTREAMER);
    cv::VideoCapture visibleCapture(irCameraPipeline, cv::CAP_GSTREAMER);

    if (!irCapture.isOpened() || (!visibleCapture.isOpened()))
    {
        std::cerr << "Error opening GStreamer Pipelines...." << std::endl;
    }
    while (previewFlag)
    {
        irCapture >> irFrames;
        visibleCapture >> visibleFrames;

        // Use to extract JUST the Y channel
        cv::Mat yChannel_IR(irFrames.rows, irFrames.cols, CV_8UC1, irFrames.data);
        cv::Mat yChannel_Vis(visibleFrames.rows, visibleFrames.cols, CV_8UC1, visibleFrames.data);

        if (irFrames.empty() || visibleFrames.empty())
        {
            std::cerr << "Empty irFrames" << std::endl;
            continue;
        }

        cv::imshow("yChannel_IR", yChannel_IR);
        cv::imshow("yChannel_Vis", yChannel_Vis);

        // Display color
        // cv::imshow("IRCamera", irFrames);
        // cv::imshow("VisibleCamera", visibleFrames);

        char key = cv::waitKey(1);

        if (key == 'q' || key == 'Q')
        {
            previewFlag = false;
        }
    }
    return 0;
}