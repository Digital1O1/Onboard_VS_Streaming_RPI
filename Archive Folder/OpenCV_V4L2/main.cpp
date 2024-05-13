#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Open the V4L2 device for the imx219 camera sensor
    VideoCapture cap("/dev/video0", CAP_V4L2);

    // Check if the camera opened successfully
    if (!cap.isOpened())
    {
        cerr << "Error opening the camera." << endl;
        return -1;
    }

    // Set the capture format to RAW
    // cap.set(CAP_PROP_FOURCC, V4L2_PIX_FMT_SRGGB10);
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // Set to MJPEG for compatibility

    // Set the capture resolution
    cap.set(CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CAP_PROP_FRAME_HEIGHT, 1080);

    // Read a single frame from the camera
    Mat frame;
    cap >> frame;

    // Check if the frame is empty
    if (frame.empty())
    {
        cerr << "Error reading the frame." << endl;
        return -1;
    }

    // Display the captured image
    // namedWindow("Single Frame", WINDOW_NORMAL);
    imshow("Single Frame", frame);

    // Wait for a key press and then exit
    waitKey(0);

    // Release the VideoCapture object and close the window
    cap.release();
    destroyAllWindows();

    return 0;
}
