#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    // Path to the YAML file
    std::string filename = "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Apply Camera Homography And Zoom To Video/build/homography_matrix.yaml";

    // Open the file using FileStorage
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return -1;
    }

    // Variables to store the camera matrix and distortion coefficients
    cv::Mat irToVisibleHomography, visibleToIRHomography;

    // Read the camera matrix and distortion coefficients from the file
    fs["irToVisibleHomography"] >> irToVisibleHomography;
    fs["visibleToIRHomography"] >> visibleToIRHomography;

    // Close the file
    fs.release();

    // Output the read values
    // std::cout << "\nirToVisibleHomography :\n"
    //           << std::endl
    //           << irToVisibleHomography << std::endl;
    // std::cout << "\nvisibleToIRHomography :\n"
    //           << std::endl
    //           << visibleToIRHomography << std::endl;

    Mat visibleToIRHomographyMatrix = visibleToIRHomography;
    Mat irToVisibleHomographyMatrix = irToVisibleHomography;

    std::cout << "PROGRAM STARTED" << std::endl;
    std::cout << "\nvisibleToIRHomographyMatrix :\n"
              << std::endl
              << visibleToIRHomographyMatrix << std::endl;

    std::cout << "\nirToVisibleHomographyMatrix :\n"
              << std::endl
              << irToVisibleHomographyMatrix << std::endl;

    // Open the default cameras for visible and IR
    VideoCapture capVisible(0); // Change the index if necessary
    VideoCapture capIR(1);      // Change the index if necessary

    // Check if the cameras opened successfully
    if (!capVisible.isOpened() || !capIR.isOpened())
    {
        std::cerr << "Error: Unable to open one of the cameras\n";
        return -1;
    }
    Mat frameVisible, frameIR, irWarpedFrame, visibleWarpedFrame;
    Mat combineRawImages, combineWarpedImages;

    // Define the region of interest (ROI) for zooming in (half size)
    Rect roiVisible(frameVisible.cols / 4, frameVisible.rows / 4, frameVisible.cols / 2, frameVisible.rows / 2);
    Rect roiIR(frameIR.cols / 4, frameIR.rows / 4, frameIR.cols / 2, frameIR.rows / 2);

    while (true)
    {
        // Mat frameVisible, frameIR;
        capVisible >> frameVisible; // Capture frame from the visible camera
        capIR >> frameIR;           // Capture frame from the IR camera

        // Check if any of the frames is empty
        if (frameVisible.empty() || frameIR.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        // Zoom in the visible frame
        Mat zoomedFrameVisible = frameVisible(roiVisible);
        resize(zoomedFrameVisible, zoomedFrameVisible, frameVisible.size());

        // Zoom in the IR frame
        Mat zoomedFrameIR = frameIR(roiIR);
        resize(zoomedFrameIR, zoomedFrameIR, frameIR.size());

        // Projects IR to VISIBLE
        // warpPerspective(frameIR, irWarpedFrame, irToVisibleHomographyMatrix, frameVisible.size());
        warpPerspective(zoomedFrameIR, irWarpedFrame, irToVisibleHomographyMatrix, frameVisible.size());

        // Projects Visible to IR this is what we want
        // warpPerspective(frameVisible, visibleWarpedFrame, visibleToIRHomography, frameIR.size());
        warpPerspective(zoomedFrameVisible, visibleWarpedFrame, visibleToIRHomography, frameIR.size());

        // cv::hconcat(frameIR, frameVisible, combineRawImages);
        // cv::hconcat(irWarpedFrame, visibleWarpedFrame, combineWarpedImages);

        // Raw images
        cv::hconcat(frameIR, frameVisible, combineRawImages);

        // Warped/zoomed in images
        cv::hconcat(irWarpedFrame, visibleWarpedFrame, combineWarpedImages);

        // Displays individually
        // cv::imshow("VISIBLE to IR", visibleWarpedFrame);
        // cv::imshow("IR to VISIBLE", irWarpedFrame);
        cv::imshow("Raw camera images (Visible | IR)", combineRawImages);
        cv::imshow("Warped camera images (Visible --> IR | IR --> Visible)", combineWarpedImages);

        // Pres ESC key to exit program
        if (waitKey(1) == 27)
        {
            std::cout << "Exiting...\n";
            break;
        }
    }

    return 0;
}
