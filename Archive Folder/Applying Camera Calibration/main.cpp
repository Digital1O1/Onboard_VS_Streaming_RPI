#include <opencv2/opencv.hpp>
#include <iostream>
#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1
int main()
{
    // Documentation reference : https://docs.opencv.org/4.x/dc/dbb/tutorial_py_calibration.html
    // Open the default camera
    cv::VideoCapture cap(NOIR_CAMERA);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to open the camera." << std::endl;
        return -1;
    }

    // Camera preview
    bool previewFlag = true;
    cv::Mat frame;

    std::cout
        << "Press 'q or Q' to continue "
        << std::endl;

    while (previewFlag)
    {
        cap >> frame;
        if (frame.empty())
        {
            std::cerr << "Empty frame" << std::endl;
            continue;
        }
        cv::imshow("Preview Window", frame);

        if (cv::waitKey(1) == 'q' || cv::waitKey(1) == 'Q')
        {
            previewFlag = false;
        }
    }

    // Camera matrix and distortion coefficients
    // NOTE: Replace these with your actual calibration results
    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 368.8844733566153, 0, 240.9157012915129,
                            0, 366.5585274717605, 256.0742999323316,
                            0, 0, 1);
    cv::Mat distCoeffs = (cv::Mat_<double>(1, 5) << -0.07906286037841737, 0.3528863781883644, -0.0295410894108972, -0.007677975646963745, -0.2850719641848566);
    // std::cout << cameraMatrix << std::endl;
    //  Capture an image from the camera
    cv::Mat capturedImage;
    cap >> capturedImage;
    if (capturedImage.empty())
    {
        std::cerr << "Error: Unable to capture an image from the camera." << std::endl;
        return -1;
    }

    // Undistort the captured image
    cv::Mat undistortedImage;
    cv::undistort(capturedImage, undistortedImage, cameraMatrix, distCoeffs);

    // Display the original and undistorted images
    cv::imshow("Captured Image", capturedImage);
    cv::imshow("Undistorted Image", undistortedImage);

    // Wait for a key press before exiting
    cv::waitKey(0);

    return 0;
}
