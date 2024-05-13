#include <opencv2/opencv.hpp>
#include "yen_threshold.h"
#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1
#define THRESHOLD_WEIGHT 0.1
#define WARPEDFRAME_WEIGHT 0.9
using namespace cv;

cv::Mat ImgProc_YenThreshold(cv::Mat src, bool compressed, double &foundThresh)
{
    // Convert frame to grayscale
    cv::Mat grey;
    cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);

    // Histogram constants
    int bins = 256;
    int histSize[] = {bins};
    const float range[] = {0, 256};
    const float *ranges[] = {range};
    int channels[] = {0};

    // equalize
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(2.7);
    cv::Mat cl;
    clahe->apply(grey, cl);

    // make histogram
    cv::Mat hist;
    cv::calcHist(&cl, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

    // yen_thresholding
    int yen_threshold = Yen(hist);
    foundThresh = yen_threshold;

    // Apply binary thresholding
    cv::Mat thresholded;
    if (compressed)
    {
        cv::threshold(cl, thresholded, double(yen_threshold), 255, cv::THRESH_BINARY);
    }
    else
    {
        cv::threshold(cl, thresholded, double(yen_threshold), 255, cv::THRESH_TOZERO);
    }

    return thresholded;
}

void debugOutput(cv::Mat visibleFrames, cv::Mat irFrames, cv::Mat zoomedIrFrames, cv::Mat zoomedVisibleFrames, cv::Mat irWarpedFrame, cv::Mat visibleWarpedFrame, cv::Mat yenThresholdedFrame)
{
    std::cout << "\n\n--------------------------------------------------------------------------------" << std::endl;
    std::cout << "visibleFrames size : \t\t" << visibleFrames.size() << "\t data type : " << visibleFrames.type() << std::endl;
    std::cout << "irFrames size : \t\t" << irFrames.size() << "\t data type : " << irFrames.type() << std::endl;
    std::cout << "zoomedIrFrames size : \t\t" << zoomedIrFrames.size() << "\t data type : " << zoomedIrFrames.type() << std::endl;
    std::cout << "zoomedVisibleFrames size : \t" << zoomedVisibleFrames.size() << "\t data type : " << zoomedVisibleFrames.type() << std::endl;
    std::cout << "irWarpedFrame size : \t\t" << irWarpedFrame.size() << "\t data type : " << irWarpedFrame.type() << std::endl;
    std::cout << "visibleWarpedFrame size : \t" << visibleWarpedFrame.size() << "\t data type : " << visibleWarpedFrame.type() << std::endl;
    std::cout << "yenThresholdedFrame size : \t" << yenThresholdedFrame.size() << "\t data type : " << yenThresholdedFrame.type() << std::endl;
}
int main()
{
    // Path to the YAML file
    std::string filename = "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Apply Camera Homography And Zoom To Video/build/homography_matrix.yaml";
    double foundThresh = 0.0;
    // Open the file using FileStorage
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return -1;
    }

    // Variables to store the camera matrix and distortion coefficients
    cv::Mat irToVisibleHomography, visibleToIRHomography, zoomedVisibleFrames, zoomedIrFrames, rawCombinedFrames, combinedZoomedFrames, combinedWarpedFrames, visibleToIRProjectedFrame, yenThresholdedFrame;

    // Read the camera matrix and distortion coefficients from the file
    fs["irToVisibleHomography"] >> irToVisibleHomography;
    fs["visibleToIRHomography"] >> visibleToIRHomography;

    // Close the file
    fs.release();

    Mat visibleToIRHomographyMatrix = visibleToIRHomography;
    Mat irToVisibleHomographyMatrix = irToVisibleHomography;

    // Output the read values
    // std::cout << "\nvisibleToIRHomographyMatrix :\n"
    //           << std::endl
    //           << visibleToIRHomographyMatrix << std::endl;
    // std::cout << "\nirToVisibleHomographyMatrix :\n"
    //           << std::endl
    //           << irToVisibleHomographyMatrix << std::endl;

    // Open the default cameras for visible and IR
    VideoCapture capIR;
    VideoCapture capVisible;
    capIR.open(NOIR_CAMERA, CAP_V4L2);
    capVisible.open(VISIBLE_CAMERA, CAP_V4L2);

    capIR.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('B', 'G', '1', '0'));
    capVisible.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('B', 'G', '1', '0'));

    // Check if the cameras opened successfully
    if (!capVisible.isOpened() || !capIR.isOpened())
    {
        std::cerr << "Error: Unable to open one of the cameras\n";
        return -1;
    }
    Mat visibleFrames, irFrames, irWarpedFrame, visibleWarpedFrame, combineRawImages, combineWarpedImages;

    while (true)
    {
        // Mat visibleFrames, irFrames;
        capVisible >> visibleFrames;
        capIR >> irFrames;

        // Check if any of the frames is empty
        if (visibleFrames.empty() || irFrames.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        // If you want to use hconcat() as a sanity check instead of viewing each frame individually
        // The following three lines of code must be commented out since visibleFrame is being grayscaled from a data type of 16 --> 0
        // Couldn't figure out how to convert the visibleFrames to 'fit' with the irFrames that has a data type of 16
        // Demosaic the frame
        Mat demosaiced;
        cvtColor(visibleFrames, demosaiced, COLOR_BayerBG2BGR); // Assuming BG Bayer pattern, adjust if necessary
        std::cout << "demosaiced " << demosaiced.size() << " type : " << demosaiced.type() << std::endl;

        // cv::resize(visibleFrames, visibleFrames, irFrames.size());
        // visibleFrames.convertTo(visibleFrames, irFrames.type());

        // // Define the region of interest (ROI) for zooming in (half size)
        // cv::Rect roiVisible(visibleFramesBGR.cols / 4, visibleFramesBGR.rows / 4, visibleFramesBGR.cols / 2, visibleFramesBGR.rows / 2);
        // cv::Rect roiIR(irFrames.cols / 4, irFrames.rows / 4, irFrames.cols / 2, irFrames.rows / 2);

        // // Zoom in the visible frame
        // zoomedVisibleFrames = visibleFrames(roiVisible);
        // zoomedIrFrames = irFrames(roiIR);

        // cv::resize(zoomedVisibleFrames, zoomedVisibleFrames, visibleFrames.size());

        // // Zoom in the IR frame
        // cv::resize(zoomedIrFrames, zoomedIrFrames, irFrames.size());

        // // Apply Yen Threshold
        // yenThresholdedFrame = ImgProc_YenThreshold(zoomedIrFrames, true, foundThresh);

        // // Projects IR to VISIBLE
        // // void cv::warpPerspective(cv::InputArray src, cv::OutputArray dst, cv::InputArray M, cv::Size dsize, int flags = 1, int borderMode = 0, const cv::Scalar &borderValue = cv::Scalar())
        // cv::warpPerspective(zoomedIrFrames, irWarpedFrame, irToVisibleHomography, zoomedVisibleFrames.size());

        // // Projects Visible to IR THIS IS WHAT WE WANT
        // cv::warpPerspective(zoomedVisibleFrames, visibleWarpedFrame, visibleToIRHomography, zoomedIrFrames.size());

        // // Uncomment if you want to see the size/resolution and data types of the frames that are being passed to debugOutput()
        // // debugOutput(visibleFrames, irFrames, zoomedIrFrames, zoomedVisibleFrames, irWarpedFrame, yenThresholdedFrame);
        // // cv::resize(visibleWarpedFrame, visibleWarpedFrame, yenThresholdedFrame.size());

        // yenThresholdedFrame.convertTo(yenThresholdedFrame, visibleWarpedFrame.type());

        // visibleWarpedFrame.convertTo(visibleWarpedFrame, CV_8U);

        // debugOutput(visibleFrames, irFrames, zoomedIrFrames, zoomedVisibleFrames, irWarpedFrame, visibleWarpedFrame, yenThresholdedFrame);

        // // void cv::addWeighted(cv::InputArray src1, double alpha, cv::InputArray src2, double beta, double gamma, cv::OutputArray dst, int dtype = -1)

        // cv::addWeighted(yenThresholdedFrame, THRESHOLD_WEIGHT, visibleWarpedFrame, WARPEDFRAME_WEIGHT, 0, visibleToIRProjectedFrame);

        // //  Concatenate  frames won't work since visibleFrames is being grayscaled
        // //  To use hconcat() those lines of code following the grayscaling of visibleFrames must be commented out 2/24/24
        // // cv::hconcat(irWarpedFrame, visibleWarpedFrame, combinedWarpedFrames);
        // // cv::hconcat(irFrames, visibleFrames, rawCombinedFrames);
        // // cv::hconcat(zoomedIrFrames, zoomedVisibleFrames, combinedZoomedFrames);
        // // cv::imshow("Raw camera feed", rawCombinedFrames);
        // // cv::imshow("Zoomed Homography camera feed ", combinedZoomedFrames);
        // // cv::imshow("Warped Frames | IR --> Visible | Visible --> IR", combinedWarpedFrames);
        // // cv::imshow("yenThresholdedFrame ", yenThresholdedFrame);

        // // Individually using imshow() to display frames seperately works fine
        // // Projects IR to VISIBLE
        // // cv::imshow("irWarpedFrame Frame", irWarpedFrame);

        // // Projects Visible to IR, this is what's implemented in CVG
        // // cv::imshow("visibleWarpedFrame Frame", visibleWarpedFrame);

        // // Projects Yen thresholded image onto the visibleframe
        // cv::imshow("Visible Frame 'Raw' ", visibleFrames);
        // cv::imshow("IR Frame 'Raw' ", irFrames);
        // cv::imshow("Combined Warped Image With Threshold Overlay ", visibleToIRProjectedFrame);

        // cv::imshow("yenThresholdedFrame ", yenThresholdedFrame);

        //  Pres ESC key to exit program
        if (waitKey(1) == 27)
        {
            std::cout << "Exiting...\n";
            break;
        }
    }

    return 0;
}
