#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // Camera indices. Update these if your cameras have different indices.
    int NOIR_Camera = 0;
    int VISIBLE_Camera = 1;

    // Create VideoCapture objects for each camera
    cv::VideoCapture cap1(NOIR_Camera);
    cv::VideoCapture cap2(VISIBLE_Camera);

    // Check if cameras opened successfully
    if (!cap1.isOpened())
    {
        std::cerr << "Error: Could not open camera " << NOIR_Camera << std::endl;
        return -1;
    }
    if (!cap2.isOpened())
    {
        std::cerr << "Error: Could not open camera " << VISIBLE_Camera << std::endl;
        return -1;
    }

    // Capture frames from each camera
    cv::Mat frame1, frame2;
    if (!cap1.read(frame1))
    {
        std::cerr << "Error: Could not read from camera " << NOIR_Camera << std::endl;
        return -1;
    }
    if (!cap2.read(frame2))
    {
        std::cerr << "Error: Could not read from camera " << VISIBLE_Camera << std::endl;
        return -1;
    }

    // File path
    std::string filePath = "/home/pi/Onboard_VS_Streaming_RPI/Capture Picture/images/";

    // Save images to disk
    if (!cv::imwrite(filePath + "NOIR_Image.jpg", frame1))
    {
        std::cerr << "Error: Could not save image from camera " << NOIR_Camera << std::endl;
        return -1;
    }
    if (!cv::imwrite(filePath + "VISIBLE_Image.jpg", frame2))
    {
        std::cerr << "Error: Could not save image from camera " << VISIBLE_Camera << std::endl;
        return -1;
    }

    std::cout << "Images captured and saved successfully." << std::endl;

    return 0;
}
