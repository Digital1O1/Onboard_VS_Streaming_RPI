#include <opencv2/opencv.hpp>
#include "yen_threshold.h"
// Function declaration
cv::Mat ImgProc_YenThreshold(cv::Mat src, bool compressed, double &foundThresh);

int main()
{
    // Open the default camera (camera 0)
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to open the camera." << std::endl;
        return -1;
    }

    while (true)
    {
        // Capture frame from the camera
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Error: Unable to capture frame." << std::endl;
            break;
        }

        // Process the frame using ImgProc_YenThreshold
        double foundThresh;
        cv::Mat processedFrame = ImgProc_YenThreshold(frame, true, foundThresh);

        // std::cout << "frame size: " << frame.size() << ", type: " << frame.type() << std::endl;
        // std::cout << "processedFrame size: " << processedFrame.size() << ", type: " << processedFrame.type() << std::endl;

        // Display the processed frame
        cv::imshow("Processed Frame", processedFrame);
        cv::imshow("Raw Frame", frame);

        // Check for ESC key press to exit
        char key = cv::waitKey(10);
        if (key == 27)
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
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
