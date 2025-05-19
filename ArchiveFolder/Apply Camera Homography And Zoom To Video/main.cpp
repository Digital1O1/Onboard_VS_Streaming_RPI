#include <opencv2/opencv.hpp>
#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1
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
    cv::Mat irToVisibleHomography, visibleToIRHomography, zoomedVisibleFrames, zoomedIrFrames, rawCombinedFrames, combinedZoomedFrames, combinedWarpedFrames;

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
    VideoCapture capIR(NOIR_CAMERA);
    VideoCapture capVisible(VISIBLE_CAMERA);

    // Check if the cameras opened successfully
    if (!capVisible.isOpened() || !capIR.isOpened())
    {
        std::cerr << "Error: Unable to open one of the cameras\n";
        return -1;
    }
    Mat visibleFrames, irFrames, irWarpedFrame, visibleWarpedFrame;
    Mat combineRawImages, combineWarpedImages;

    // Mat visibleFrames, irFrames, irWarpedFrame, visibleWarpedFrame;

    while (true)
    {
        // Mat visibleFrames, irFrames;
        capVisible >> visibleFrames; // Capture frame from the visible camera
        capIR >> irFrames;           // Capture frame from the IR camera

        // Check if any of the frames is empty
        if (visibleFrames.empty() || irFrames.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        // Define the region of interest (ROI) for zooming in (half size)
        cv::Rect roiVisible(visibleFrames.cols / 4, visibleFrames.rows / 4, visibleFrames.cols / 2, visibleFrames.rows / 2);
        cv::Rect roiIR(irFrames.cols / 4, irFrames.rows / 4, irFrames.cols / 2, irFrames.rows / 2);

        // Zoom in the visible frame
        zoomedVisibleFrames = visibleFrames(roiVisible);
        cv::resize(zoomedVisibleFrames, zoomedVisibleFrames, visibleFrames.size());

        // Zoom in the IR frame
        zoomedIrFrames = irFrames(roiIR);
        cv::resize(zoomedIrFrames, zoomedIrFrames, irFrames.size());

        // cv::hconcat(irFrames, visibleFrames, rawCombinedFrames);
        // cv::hconcat(zoomedIrFrames, zoomedVisibleFrames, rawCombinedFrames);

        cv::hconcat(irFrames, visibleFrames, rawCombinedFrames);                // concatenate raw frames
        cv::hconcat(zoomedIrFrames, zoomedVisibleFrames, combinedZoomedFrames); // concatenate zoomed frames

        // Projects IR to VISIBLE
        cv::warpPerspective(zoomedIrFrames, irWarpedFrame, irToVisibleHomography, zoomedVisibleFrames.size());

        // Projects Visible to IR
        cv::warpPerspective(zoomedVisibleFrames, visibleWarpedFrame, visibleToIRHomography, zoomedIrFrames.size());

        // Concatenate warped frames
        cv::hconcat(irWarpedFrame, visibleWarpedFrame, combinedWarpedFrames); // concatenate warped frames

        cv::imshow("Raw camera feed", rawCombinedFrames);
        cv::imshow("Zoomed Homography camera feed ", combinedZoomedFrames);
        cv::imshow("Warped Frames | IR --> Visible | Visible --> IR", combinedWarpedFrames);

        // Pres ESC key to exit program
        if (waitKey(1) == 27)
        {
            std::cout << "Exiting...\n";
            break;
        }

        // cv::imwrite("RawVisibleFrameZoomed.PNG", zoomedVisibleFrames);
        //  cv::imwrite("RawIRFrameZoomed", zoomedIrFrames);
        //  cv::imwrite("VisibleWarpedFrame", visibleWarpedFrame);
        //  cv::imwrite("IRToVisibleWarpedFrame", irWarpedFrame);
    }

    return 0;
}
