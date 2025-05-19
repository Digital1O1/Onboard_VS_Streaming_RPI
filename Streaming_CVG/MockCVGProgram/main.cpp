#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "yen_threshold.h"

#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1
#define CHESSBOARD_COLUMNS 9
#define CHESSBOARD_ROWS 6
#define LINE_THICKNESS 1
#define NUMBER_OF_CALIBRATION_IMAGES 1
#define CALIBRATION_DELAY 1000 // In milliseconds
#define THRESHOLD_WEIGHT 0.4   // Increase to see more of the Yen Threshold Image
#define WARPEDFRAME_WEIGHT 0.6
#define ESC_KEY 27

using namespace cv;

void remap_lut_threshold(cv::Mat &src, cv::Mat &dst, float k, int threshold, int &topkvalue)
{
    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange = {range};
    cv::Mat hist;
    cv::calcHist(&src, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    int high = 0;
    int low = 0;
    for (int i = 0; i < 256; i++)
    {
        if (hist.at<float>(i) != 0)
        {
            high = i;
            if (low == 0)
                low = i;
        }
    }

    cv::Mat histCumulative = hist.clone();
    for (int i = 1; i < histSize; i++)
    {
        histCumulative.at<float>(i) += histCumulative.at<float>(i - 1);
    }
    float abovePixels = histCumulative.at<float>(255) - histCumulative.at<float>(threshold);
    float totalPixels = static_cast<float>(src.rows * src.cols);
    topkvalue = 0;
    for (int i = 0; i < histSize; i++)
    {
        if (histCumulative.at<float>(i) >= totalPixels - k * abovePixels)
        {
            topkvalue = i;
            break;
        }
    }

    cv::Mat lut(1, 256, CV_8U);
    for (int i = 0; i <= 255; i++)
    {
        if (i > topkvalue)
            lut.at<uchar>(0, i) = 255;
        else if (i > threshold)
            lut.at<uchar>(0, i) = static_cast<uchar>(255.0 * (i - threshold) / (high - threshold));
        else
            lut.at<uchar>(0, i) = 0;
    }

    cv::LUT(src, lut, dst);
}

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
    foundThresh = static_cast<double>(yen_threshold);

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

int main()
{
    // Paths to the YAML files
    std::string filename = "/home/pi/Desktop/CVG-Tietronix/PythonCameraScript/C++ManualAdjustment/ProfusionProject/LinuxFolder/AlignImages/homography_2025-05-16_09:42:07.yml";
    std::string manualTransformation = "/home/pi/Desktop/CVG-Tietronix/PythonCameraScript/C++ManualAdjustment/ProfusionProject/LinuxFolder/AlignImages/manual_homography.yml";

    std::cout << "Opening files: " << filename << "\n and \n" << manualTransformation << std::endl;

    // Open the file using FileStorage
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        std::cerr << "\n\nFailed to open " << filename << std::endl;
        return -1;
    }

    cv::FileStorage manualTransform(manualTransformation, cv::FileStorage::READ);
    if (!manualTransform.isOpened())
    {
        std::cerr << "\n\nFailed to open " << manualTransformation << std::endl;
        return -1;
    }

    cv::Mat visibleToInfraredHomography, manualTransformMatrix;
    fs["transformation"] >> visibleToInfraredHomography;
    manualTransform["manual_transformation"] >> manualTransformMatrix;
    fs.release();
    manualTransform.release();

    if (visibleToInfraredHomography.empty())
    {
        std::cerr << "Failed to read visibleToInfraredHomography matrix from file" << std::endl;
        return -1;
    }
    else
        std::cout << "\nHomography file loaded successfully \r\n";

    if (manualTransformMatrix.empty())
    {
        std::cerr << "Failed to read manualTransformMatrix from file" << std::endl;
        return -1;
    }
    else
        std::cout << "Manual Transformation matrix loaded successfully \r\n";

    // Open cameras
    cv::VideoCapture capIR(NOIR_CAMERA);
    cv::VideoCapture capVisible(VISIBLE_CAMERA);

    capIR.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('B', 'G', '1', '0'));
    capVisible.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('B', 'G', '1', '0'));

    capIR.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    capIR.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    capVisible.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    capVisible.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if (!capIR.isOpened() || !capVisible.isOpened())
    {
        std::cerr << "Error: Cameras not accessible" << std::endl;
        return -1;
    }

    std::cout << "\nPress ESC key to exit...\r\n";

    cv::Mat irFrames, visibleFrames;
    cv::Mat croppedVisibleFrames, croppedIrFrames;
    cv::Mat visibleWarpedInitial, visibleWarpedFrame, processedFrame, yenThresholdedFrame;
    cv::Mat visibleFramesBGR, visibleToIRProjectedFrame;

    double foundThresh = 0.0;
    int topkvalue = 0;
    int remapMin = 0;

    while (true)
    {
        capVisible >> visibleFrames;
        capIR >> irFrames;
        cv::flip(irFrames, irFrames, 1);

        if (visibleFrames.empty() || irFrames.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        // Convert visibleFrames to grayscale then back to BGR for display & processing
        cv::cvtColor(visibleFrames, visibleFrames, cv::COLOR_BGR2GRAY);
        cv::cvtColor(visibleFrames, visibleFramesBGR, cv::COLOR_GRAY2BGR);

        // Define ROI (zoom)
        cv::Rect roiVisible(visibleFramesBGR.cols / 4, visibleFramesBGR.rows / 4, visibleFramesBGR.cols / 2, visibleFramesBGR.rows / 2);
        cv::Rect roiIR(irFrames.cols / 4, irFrames.rows / 4, irFrames.cols / 2, irFrames.rows / 2);

        croppedVisibleFrames = visibleFramesBGR(roiVisible);
        croppedIrFrames = irFrames(roiIR);

        cv::resize(croppedVisibleFrames, croppedVisibleFrames, visibleFrames.size());
        cv::resize(croppedIrFrames, croppedIrFrames, irFrames.size());

        yenThresholdedFrame = ImgProc_YenThreshold(croppedIrFrames, false, foundThresh);
        remapMin = static_cast<int>(foundThresh);

        remap_lut_threshold(yenThresholdedFrame, processedFrame, 0.1f, remapMin, topkvalue);

        cv::Ptr<cv::Mat> ColoredFrame = cv::Ptr<cv::Mat>(new cv::Mat());
        cv::applyColorMap(processedFrame, *ColoredFrame, cv::COLORMAP_JET);

        // Add the yenThresholdedFrame as alpha channel
        std::vector<cv::Mat> channels;
        cv::split(*ColoredFrame, channels);
        channels.push_back(yenThresholdedFrame);
        cv::merge(channels, *ColoredFrame);

        // Warp visible frame using visibleToInfraredHomography (automatic alignment)
        cv::warpPerspective(croppedVisibleFrames, visibleWarpedInitial, visibleToInfraredHomography, croppedIrFrames.size());

        // Apply manualTransformMatrix for manual fine adjustment
        cv::warpPerspective(visibleWarpedInitial, visibleWarpedFrame, manualTransformMatrix, croppedIrFrames.size());

        visibleWarpedFrame.convertTo(visibleWarpedFrame, (*ColoredFrame).type());
        cv::cvtColor(visibleWarpedFrame, visibleWarpedFrame, cv::COLOR_BGR2BGRA);

        // Blend warped visible frame and colored IR image
        //cv::addWeighted(*ColoredFrame, THRESHOLD_WEIGHT, visibleWarpedFrame, WARPEDFRAME_WEIGHT, 0, visibleToIRProjectedFrame);


        // Ensure both are 4-channel
cv::Mat visibleBGRA;
if (visibleWarpedFrame.channels() == 3)
    cv::cvtColor(visibleWarpedFrame, visibleBGRA, cv::COLOR_BGR2BGRA);
else
    visibleBGRA = visibleWarpedFrame.clone();

// Split channels
std::vector<cv::Mat> irChannels(4), visChannels(4);
cv::split(*ColoredFrame, irChannels);
cv::split(visibleBGRA, visChannels);

// Alpha channel from IR
cv::Mat irAlpha;
irChannels[3].convertTo(irAlpha, CV_32F, 1.0 / 255.0);
cv::Mat invAlpha = 1.0 - irAlpha;

// Blend R, G, B channels
std::vector<cv::Mat> blendedChannels(3);
for (int c = 0; c < 3; ++c) {
    cv::Mat irF, visF;
    irChannels[c].convertTo(irF, CV_32F);
    visChannels[c].convertTo(visF, CV_32F);
    blendedChannels[c] = irF.mul(irAlpha) + visF.mul(invAlpha);
    blendedChannels[c].convertTo(blendedChannels[c], CV_8U);
}

// Merge back into final output (3-channel BGR)
cv::merge(blendedChannels, visibleToIRProjectedFrame);



        // Display windows
        //cv::imshow("Visible ROI", croppedVisibleFrames);
        //cv::imshow("IR ROI", croppedIrFrames);
        //cv::imshow("Yen Threshold", yenThresholdedFrame);
        //cv::imshow("Color Map", *ColoredFrame);
        //cv::imshow("Warped Visible Initial", visibleWarpedInitial);
        //cv::imshow("Warped Visible Final (with manual adjustment)", visibleWarpedFrame);
        cv::imshow("Overlay Visible to IR", visibleToIRProjectedFrame);

        int key = cv::waitKey(5);
        if (key == ESC_KEY)
            break;
    }

    capIR.release();
    capVisible.release();
    cv::destroyAllWindows();
    return 0;
}

