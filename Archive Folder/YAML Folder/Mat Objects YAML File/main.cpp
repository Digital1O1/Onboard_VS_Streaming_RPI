#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // Your cv::Mat object
    cv::Mat irToVisibleHomography = (cv::Mat_<float>(3, 3) << 0.9534022142936324, 0.01305481316787236, 118.1218640133471,
                                     0.00627692754899878, 0.9683982081727268, 13.3468227972566,
                                     6.119384756992607e-06, -7.330168272316063e-05, 1);

    cv::Mat visibleToIRHomography = (cv::Mat_<float>(3, 3) << 1.051327970696641, -0.02378592953453893, -123.9886093390048,
                                     -0.006268605731544748, 1.03247656271846, -13.15074739500794,
                                     -4.907019775836022e-06, 7.500948560563594e-05, 1);
    // Specify the filename
    std::string filename = "homography_matrix.yaml";

    // Create a FileStorage object in write mode
    cv::FileStorage saveFS(filename, cv::FileStorage::WRITE);

    // Check if it is opened properly
    if (!saveFS.isOpened())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return -1;
    }

    // Write the matrix to the file
    saveFS << "irToVisibleHomography" << irToVisibleHomography;
    saveFS << "visibleToIRHomography" << visibleToIRHomography;

    // Release the file storage
    saveFS.release();

    std::cout << "Matrix written to " << filename << std::endl;

    // Specify the filename
    std::string readFileName = "homography_matrix.yaml";

    // Create a FileStorage object in read mode
    cv::FileStorage readFS(readFileName, cv::FileStorage::READ);

    // Check if it is opened properly
    if (!readFS.isOpened())
    {
        std::cerr << "Failed to open " << readFileName << std::endl;
        return -1;
    }

    // Declare a matrix to store the data
    cv::Mat readirToVisibleHomography, readVisibleToIRHomography;

    // Read the matrix from the file
    readFS["irToVisibleHomography"] >> readirToVisibleHomography;
    readFS["visibleToIRHomography"] >> readVisibleToIRHomography;

    // Check if the matrix is not empty
    if (readirToVisibleHomography.empty() || readVisibleToIRHomography.empty())
    {
        std::cerr << "Matrix not found or is empty in the file" << std::endl;
        return -1;
    }

    // Display the read matrix
    std::cout << "readirToVisibleHomography matrix:\n\n"
              << readirToVisibleHomography << std::endl;

    std::cout << "\n\nvisibleToIRHomography matrix:\n\n"
              << visibleToIRHomography << std::endl;

    // Release the file storage
    readFS.release();

    return 0;
}
