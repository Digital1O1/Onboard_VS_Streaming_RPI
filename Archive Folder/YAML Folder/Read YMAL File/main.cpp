// READ YAML FILE

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main()
{
    // Path to the YAML file
    std::string filename = "/home/pi/Onboard_VS_Streaming_RPI/Read YMAL File/intrinsic.yaml";

    // Open the file using FileStorage
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return -1;
    }

    // Variables to store the camera matrix and distortion coefficients
    cv::Mat NOIR_cameraMatrix, NOIR_distCoeffs, VISIBLE_cameraMatrix, VISIBLE_distCoeffs;

    // Read the camera matrix and distortion coefficients from the file
    fs["NOIR_camera_matrix"] >> NOIR_cameraMatrix;
    fs["NOIR_distortion_coefficients"] >> NOIR_distCoeffs;
    fs["Visible_camera_matrix"] >> VISIBLE_cameraMatrix;
    fs["Visible_distortion_coefficients"] >> VISIBLE_distCoeffs;
    // Close the file
    fs.release();

    // Output the read values
    std::cout << "\nNOIR Camera Matrix :\n"
              << std::endl
              << NOIR_cameraMatrix << std::endl;
    std::cout << "\nNOIR Distortion Coefficients :\n"
              << std::endl
              << NOIR_distCoeffs << std::endl;
    std::cout << "\nVISIBLE Camera Matrix :\n"
              << std::endl
              << VISIBLE_cameraMatrix << std::endl;
    std::cout << "\nVISIBLE Distortion Coefficients :\n"
              << std::endl
              << VISIBLE_distCoeffs << std::endl;
    return 0;
}
