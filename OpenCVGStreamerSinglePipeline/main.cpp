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
    // Place Linux Laptop IP address here
    std::string sendToLinuxLaptopPipeline =
        "appsrc ! videoconvert ! v4l2h264enc ! rtph264pay config-interval=10 pt=96 ! udpsink host=172.17.140.56 port=5001";

    std::string recieveFromLinuxLaptopPipeline =
        "udpsrc address=172.17.141.124 port=5002 caps=\"application/x-rtp, encoding-name=H264, payload=96\" ! "
        "rtph264depay ! h264parse ! queue ! v4l2h264dec ! videoconvert ! appsink sync=false";

    cv::VideoWriter sendToLinuxLaptop(
        sendToLinuxLaptopPipeline,
        cv::VideoWriter::fourcc('H', '2', '6', '4'),
        30,
        cv::Size(640, 480),
        true);

    cv::VideoCapture receiver(
        recieveFromLinuxLaptopPipeline,
        cv::CAP_GSTREAMER);

    if (!sendToLinuxLaptop.isOpened())
    {
        std::cerr << "Error: Unable to open the GStreamer pipeline" << std::endl;
        return -1;
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
        sendToLinuxLaptop.write(rawFrameRPI);

        receiver.read(linuxFrame);
        if (!linuxFrame.empty())
        {
            std::cerr << "Error: Received empty frame" << std::endl;
            continue; // Skip to the next loop iteration
        }
        cv::imshow("Video Feed From RPI", rawFrameRPI);

        cv::imshow("Video Feed From Linux Laptop", linuxFrame);
        if (cv::waitKey(1) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
