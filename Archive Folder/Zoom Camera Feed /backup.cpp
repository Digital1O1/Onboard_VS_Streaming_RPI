#include <opencv2/opencv.hpp>

int main()
{
    // Open the default camera (camera 0)
    cv::VideoCapture cap(1);

    // Check if the camera opened successfully
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to access camera" << std::endl;
        return -1;
    }

    // Set desired dimensions for the video frame
    int frameWidth = 640;
    int frameHeight = 480;

    // Set the zoom parameters
    int zoomFactor = 2; // Zoom factor (2x zoom)
    int zoomedWidth = frameWidth;
    int zoomedHeight = frameHeight;

    // Create a window to display the video feed
    cv::namedWindow("Zoomed Video Feed", cv::WINDOW_NORMAL);
    cv::resizeWindow("Zoomed Video Feed", zoomedWidth, zoomedHeight);

    // Loop to continuously capture and display frames from the camera
    while (true)
    {
        // Capture frame from the camera
        cv::Mat frame;
        cap >> frame;

        // Check if the frame is empty
        if (frame.empty())
        {
            std::cerr << "Error: Unable to capture frame" << std::endl;
            break;
        }

        // Resize the frame to achieve zoom effect
        cv::Mat zoomedFrame;
        cv::resize(frame, zoomedFrame, cv::Size(zoomedWidth, zoomedHeight));

        // Crop the center portion to maintain the desired resolution
        int x = (zoomedFrame.cols - frameWidth) / 2;
        int y = (zoomedFrame.rows - frameHeight) / 2;
        cv::Rect roi(x, y, frameWidth, frameHeight);
        zoomedFrame = zoomedFrame(roi);

        // Display the zoomed frame
        cv::imshow("Zoomed Video Feed", zoomedFrame);
        cv::imshow("Regular Video Feed", frame);

        // Check for user input to exit the program
        if (cv::waitKey(1) == 27) // 27 is the ASCII code for 'Esc'
            break;
    }

    // Release the camera and close all OpenCV windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
