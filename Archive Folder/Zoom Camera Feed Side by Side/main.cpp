#include <opencv2/opencv.hpp>
#define IR_CAMERA 0
#define VISIBLE_CAMERA 1
using namespace cv;

int main()
{
    // Open the default cameras for visible and IR
    VideoCapture capVisible(VISIBLE_CAMERA);
    VideoCapture capIR(IR_CAMERA);

    // Check if the cameras opened successfully
    if (!capVisible.isOpened() || !capIR.isOpened())
    {
        std::cerr << "Error: Unable to open one of the cameras\n";
        return -1;
    }

    // Create a window to display the video
    namedWindow("Combined Frames", WINDOW_NORMAL);

    // Loop to continuously capture frames from the cameras
    while (true)
    {
        Mat frameVisible, frameIR;
        capVisible >> frameVisible; // Capture frame from the visible camera
        capIR >> frameIR;           // Capture frame from the IR camera

        // Check if any of the frames is empty
        if (frameVisible.empty() || frameIR.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        // Define the region of interest (ROI) for zooming in (half size)
        Rect roiVisible(frameVisible.cols / 4, frameVisible.rows / 4, frameVisible.cols / 2, frameVisible.rows / 2);
        Rect roiIR(frameIR.cols / 4, frameIR.rows / 4, frameIR.cols / 2, frameIR.rows / 2);

        // Zoom in the visible frame
        Mat zoomedFrameVisible = frameVisible(roiVisible);
        resize(zoomedFrameVisible, zoomedFrameVisible, frameVisible.size());

        // Zoom in the IR frame
        Mat zoomedFrameIR = frameIR(roiIR);
        resize(zoomedFrameIR, zoomedFrameIR, frameIR.size());

        // Concatenate all frames into one matrix
        Mat combinedFrame;
        hconcat(frameVisible, zoomedFrameVisible, frameVisible);
        hconcat(frameIR, zoomedFrameIR, frameIR);
        vconcat(frameVisible, frameIR, combinedFrame);

        // Display the combined frame
        imshow("Combined Frames", combinedFrame);

        // Check for user input to exit
        if (waitKey(1) == 27)
        {
            std::cout << "Exiting...\n";
            break;
        }
    }

    // Release the cameras and destroy windows
    capVisible.release();
    capIR.release();
    destroyAllWindows();

    return 0;
}
