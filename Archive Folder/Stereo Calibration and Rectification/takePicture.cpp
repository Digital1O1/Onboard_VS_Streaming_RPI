#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // Camera indices. Update these if your cameras have different indices.
    int cameraIndex1 = 0;
    int cameraIndex2 = 1;

    // Create VideoCapture objects for each camera
    cv::VideoCapture cap1(cameraIndex1);
    cv::VideoCapture cap2(cameraIndex2);

    // Check if cameras opened successfully
    if (!cap1.isOpened())
    {
        std::cerr << "Error: Could not open camera " << cameraIndex1 << std::endl;
        return -1;
    }
    if (!cap2.isOpened())
    {
        std::cerr << "Error: Could not open camera " << cameraIndex2 << std::endl;
        return -1;
    }

    // Capture frames from each camera
    cv::Mat frame1, frame2;
    if (!cap1.read(frame1))
    {
        std::cerr << "Error: Could not read from camera " << cameraIndex1 << std::endl;
        return -1;
    }
    if (!cap2.read(frame2))
    {
        std::cerr << "Error: Could not read from camera " << cameraIndex2 << std::endl;
        return -1;
    }

    // Save images to disk
    if (!cv::imwrite("image_from_camera_1.jpg", frame1))
    {
        std::cerr << "Error: Could not save image from camera " << cameraIndex1 << std::endl;
        return -1;
    }
    if (!cv::imwrite("image_from_camera_2.jpg", frame2))
    {
        std::cerr << "Error: Could not save image from camera " << cameraIndex2 << std::endl;
        return -1;
    }

    std::cout << "Images captured and saved successfully." << std::endl;

    return 0;
}
