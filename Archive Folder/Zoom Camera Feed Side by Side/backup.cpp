#include <opencv2/opencv.hpp>
#define IR_CAMERA 0
#define VISIBLE_CAMERA 1
using namespace cv;

int main()
{
    // Open the default camera
    VideoCapture cap(0);

    // Check if the camera opened successfully
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to open the camera\n";
        return -1;
    }

    // Create a window to display the video
    namedWindow("Original and Zoomed Frames", WINDOW_NORMAL);

    // Loop to continuously capture frames from the camera
    while (true)
    {
        Mat frame;
        cap >> frame; // Capture frame from the camera

        // Check if the frame is empty
        if (frame.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        // Define the region of interest (ROI) for zooming in
        // Rect(int x, int y, int width, int height)
        Rect roi(frame.cols / 2, frame.rows / 2, frame.cols / 2, frame.rows / 2);

        // Size of ROI : [320 x 240 from (160, 120)]
        // std::cout << "Size of ROI : " << roi << std::endl;
        Mat zoomedFrame = frame(roi);

        // Resize the zoomed frame to the original size
        resize(zoomedFrame, zoomedFrame, frame.size());

        // Create a larger Mat to accommodate both frames side by side
        Mat displayFrame(frame.rows, frame.cols * 2, frame.type());

        // Copy the original frame and the zoomed-in frame into their respective regions
        frame.copyTo(displayFrame(Rect(0, 0, frame.cols, frame.rows)));
        zoomedFrame.copyTo(displayFrame(Rect(frame.cols, 0, frame.cols, frame.rows)));

        // Display the original and zoomed frames side by side
        imshow("Original and Zoomed Frames", displayFrame);

        // Check for user input to exit
        if (waitKey(1) == 27)
        {
            std::cout << "Exiting...\n";
            break;
        }
    }

    // Release the camera and destroy windows
    cap.release();
    destroyAllWindows();

    return 0;
}
