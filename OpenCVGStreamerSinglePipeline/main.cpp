#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::VideoCapture cap(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    // Check if the camera is opened successfully
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to access the camera" << std::endl;
        return -1;
    }

    /*
    gst-launch-1.0 libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! \
    capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! v4l2convert ! queue ! \
    v4l2h264enc extra-controls="controls,repeat_sequence_header=1" ! \
    'video/x-h264,level=(string)4.2,profile=(string)baseline' ! h264parse ! rtph264pay ! \
    queue ! udpsink host=$HOST_LAPTOP port=$PORT_CAM1 &
    */
    // Place Linux Laptop IP address here
    std::string sendToLinuxLaptopPipeline =
        "libcamerasrc camera-name=\"/base/soc/i2c0mux/i2c@0/imx219@10\" ! "
        "capsfilter caps=video/x-raw,width=640,height=480,format=NV12 ! "
        "v4l2convert ! "
        "queue ! "
        "videoconvert ! "
        "appsink";       

    std::string recieveFromLinuxLaptopPipeline =
        "udpsrc address=172.17.141.124 port=5002 caps=\"application/x-rtp, encoding-name=H264, payload=96\" ! "
        "rtph264depay ! h264parse ! queue ! v4l2h264dec ! videoconvert ! appsink sync=false";

    cv::VideoCapture sendToLinuxLaptop(sendToLinuxLaptopPipeline, cv::CAP_GSTREAMER);

    // cv::VideoWriter sendToLinuxLaptop(
    //     sendToLinuxLaptopPipeline,
    //     cv::VideoWriter::fourcc('H', '2', '6', '4'),
    //     30,
    //     cv::Size(640, 480),
    //     true);

    // cv::VideoCapture receiver(
    //     recieveFromLinuxLaptopPipeline,
    //     cv::CAP_GSTREAMER);

    if (!sendToLinuxLaptop.isOpened())
    {
        std::cerr << "Error: Unable to open the GStreamer pipeline" << std::endl;
        return -1;
    }
    else
    {
        printf("Open\r\n");
    }

    cv::Mat rawFrameRPI, linuxFrame;

    while (true)
    {
        cap >> rawFrameRPI;

        if (rawFrameRPI.empty())
        {
            std::cerr << "Error: Unable to capture a frame" << std::endl;
            break;
        }

        // Display the frame in the window
        // cv::imshow("Video Feed From RPI", rawFrameRPI);
        // sendToLinuxLaptop.write(rawFrameRPI);

        // receiver.read(linuxFrame);
        // if (!linuxFrame.empty())
        // {
        //     std::cerr << "Error: Received empty frame" << std::endl;
        //     continue; // Skip to the next loop iteration
        // }
        cv::imshow("Video Feed From RPI", rawFrameRPI);

        // cv::imshow("Video Feed From Linux Laptop", linuxFrame);
        if (cv::waitKey(1) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
