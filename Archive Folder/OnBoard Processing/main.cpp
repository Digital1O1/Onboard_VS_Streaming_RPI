#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <inttypes.h>
#include "yen_threshold.h"
int main()
{
    cv::VideoCapture cap("/dev/video0", cv::CAP_V4L2);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera." << std::endl;
        return 1;
    }
    // Set the capture format to RAW
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('R', 'G', '1', '0'));

    // Set the camera resolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    auto capture_image_START = std::chrono::high_resolution_clock::now();

    cv::Mat cam0_frame;
    cap >> cam0_frame;
    auto capture_image_END = std::chrono::high_resolution_clock::now();
    auto capture_image_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(capture_image_END - capture_image_START);
    printf("\n[STEP 1] Capture image : %" PRId64 " MICROSECONDS\r\n", capture_image_DURATION.count());

    if (cam0_frame.empty())
    {
        std::cerr << "Error: Frame is empty." << std::endl;
        return 1;
    }
    int resolution[2];
    resolution[0] = cam0_frame.cols;
    resolution[1] = cam0_frame.rows;

    // 8-bit unsigned integer matrix with a single channel
    cv::Mat rawImageMat(resolution[1], resolution[0], CV_8UC1);

    auto convert_To_GrayScale_START = std::chrono::high_resolution_clock::now();
    // Convert the input image to single-channel grayscale
    cv::cvtColor(cam0_frame, rawImageMat, cv::COLOR_BGR2GRAY);
    auto convert_To_GrayScale_END = std::chrono::high_resolution_clock::now();
    auto convert_To_GrayScale_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(convert_To_GrayScale_END - convert_To_GrayScale_START);
    printf("\n[STEP 2]convert_To_GrayScale_DURATION : %" PRId64 " MICROSECONDS\r\n", convert_To_GrayScale_DURATION.count());

    auto demosaicedFrame_START = std::chrono::high_resolution_clock::now();
    cv::Mat demosaicedFrame;
    // Apply the demosaicing algorithm
    cv::cvtColor(rawImageMat, demosaicedFrame, cv::COLOR_BayerRG2BGR);
    auto demosaicedFrame_END = std::chrono::high_resolution_clock::now();
    auto demosaicedFrame_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(demosaicedFrame_END - demosaicedFrame_START);
    printf("\n[STEP 3]demosaicedFrame_DURATION : %" PRId64 " MICROSECONDS\r\n", demosaicedFrame_DURATION.count());

    auto grayImage_START = std::chrono::high_resolution_clock::now();
    cv::Mat grayImage;
    // Convert demosiaic to grayscale for adaptive thresholding
    cv::cvtColor(demosaicedFrame, grayImage, cv::COLOR_BGR2GRAY);
    auto grayImage_END = std::chrono::high_resolution_clock::now();
    auto grayImage_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(grayImage_END - grayImage_START);
    printf("\n[STEP 4]grayScale_DURATION : %" PRId64 " MICROSECONDS\r\n", grayImage_DURATION.count());

    // auto thresholdedImage_START = std::chrono::high_resolution_clock::now();
    // cv::Mat thresholdedImage;
    // // adaptiveThreshold(src, dst, maxValue, adaptiveMethod, thresholdType, blockSize, C)
    // cv::adaptiveThreshold(grayImage, thresholdedImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
    // auto thresholdedImage_END = std::chrono::high_resolution_clock::now();
    // auto thresholdedImage_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(thresholdedImage_END - thresholdedImage_START);
    // printf("\n[STEP 5]thresholdedImage_DURATION : %" PRId64 " MICROSECONDS\r\n", thresholdedImage_DURATION.count());

    // -------------------------------------------- [ YEN THRESHOLDING ] -------------------------------------------- //
    // cv::Mat yenImage;

    // Histogram constants
    //  Quantize the hue to 30 levels
    //  and the saturation to 32 levels
    int bins = 256;
    int histSize[] = {bins};
    // hue varies from 0 to 179, see cvtColor
    const float range[] = {0, 256};
    const float *ranges[] = {range};
    cv::Mat hist;
    // we compute the histogram from the 0-th channel
    int channels[] = {0};

    // equalize
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(2.7);
    cv::Mat cl;
    clahe->apply(grayImage, cl);

    // make histogram
    calcHist(&cl, 1, channels, cv::Mat(), // do not use mask
             hist, 1, histSize, ranges,
             true, // the histogram is uniform
             false);

    // yen_thresholding
    int yen_threshold = Yen(hist);
    double foundThresh = 0.0;
    foundThresh = yen_threshold;
    cv::Mat thresholded;
    std::cout << "Yen threshold : " << yen_threshold << "\n";
    // Not compressed
    cv::threshold(
        cl,
        thresholded,
        double(yen_threshold),
        255,
        cv::THRESH_TOZERO);

    // blur
    cv::Mat blurred;
    cv::medianBlur(thresholded, blurred, 7);

    // Note the next steps are expensive and possibly unnecesaary keeping them for completeness
    // edges
    cv::Mat edges;
    cv::Canny(blurred, edges, 120, 120);

    // dialate
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3), cv::Point(1, 1));
    cv::Mat dialated;
    cv::dilate(edges, dialated, kernel);

    // flood
    cv::Mat flooded = dialated.clone();
    cv::floodFill(flooded, cv::Point(0, 0), cv::Scalar(255));

    // invert
    cv::Mat invert;
    cv::bitwise_not(flooded, invert);
    cv::Mat inverted = cv::Mat(invert);

    // -------------------------------------------- [ DISPLAY IMAGES ] -------------------------------------------- //
    cv::imshow("cam0_frame", cam0_frame);
    // cv::imshow("rawImageMat", rawImageMat);
    // cv::imshow("demosaicedFrame", demosaicedFrame);
    // cv::imshow("grayImage", grayImage);
    // cv::imshow("thresholdedImage", thresholdedImage);
    cv::imshow("inverted", inverted);

    cv::waitKey(0);
    return 0;
}
