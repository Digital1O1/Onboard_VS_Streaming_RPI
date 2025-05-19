#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

/*
    Notes on why chessboard is still needed despite camera calibration matrix and distortion coefficients are pre-calculated
        - Stereocalibration involves more than just lens distortion
            - It's about finding relative position and orientation; the EXTRINSIC PARAMETERS, between the two cameras
            - Both important for computing depth and stereo images

        - Chessboard image used to find corresponding points between the two camera views
            - Done by observing how these points from the 3d world that's projected onto each camera's 2D image plane
            - Stereo calibration process can estimate relative geometry of the two cameras
                - Mainly rotation 'R' and translation 'T'
                - Describes how to transform points from one camera's coordinate system to the other's

        - Once you have the relative geometry along with individual camera's intrinsic parameters
            - You can rectify the images of from the two cameras
            - Rectification
                - Involves warping the images from both cameras so their corresponding epipolar lines become aligned
                    - Epipolar lines
                - Very important step for computing disparity maps for 3D reconstruction
                - Simplifies finding corresponding points between two images in a 1D search along the rows
*/

// Function to perform stereo calibration and rectification
void stereoCalibrateAndRectify(const std::vector<std::string> &imagePaths1,
                               const std::vector<std::string> &imagePaths2,
                               cv::Size boardSize,
                               const cv::Mat &noIRCameraMatrix,
                               const cv::Mat &noIRDistCoeff,
                               const cv::Mat &visibleCameraMatrix,
                               const cv::Mat &visibleCoeff,
                               cv::Mat &R1, cv::Mat &R2, cv::Mat &P1, cv::Mat &P2)
{
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints1, imagePoints2;
    cv::Mat image1;
    cv::Mat image2;

    std::vector<cv::Point3f> objp;
    for (int i = 0; i < boardSize.height; ++i)
    {
        for (int j = 0; j < boardSize.width; ++j)
        {
            objp.emplace_back(j, i, 0.0f);
        }
    }

    for (size_t i = 0; i < imagePaths1.size(); ++i)
    {
        image1 = cv::imread(imagePaths1[i]);
        image2 = cv::imread(imagePaths2[i]);

        std::vector<cv::Point2f> corners1, corners2;
        bool found1 = cv::findChessboardCorners(image1, boardSize, corners1);
        bool found2 = cv::findChessboardCorners(image2, boardSize, corners2);

        if (found1 && found2)
        {
            objectPoints.push_back(objp);
            imagePoints1.push_back(corners1);
            imagePoints2.push_back(corners2);
        }
        else
        {
            printf("Chessboard not round...\r\n");
        }
    }

    cv::Mat R, T, E, F;
    cv::stereoCalibrate(objectPoints, imagePoints1, imagePoints2,
                        noIRCameraMatrix, noIRDistCoeff,
                        visibleCameraMatrix, visibleCoeff,
                        image1.size(), R, T, E, F);

    cv::stereoRectify(noIRCameraMatrix, noIRDistCoeff,
                      visibleCameraMatrix, visibleCoeff,
                      image1.size(), R, T, R1, R2, P1, P2,
                      cv::noArray(), cv::CALIB_ZERO_DISPARITY);
}

int main()
{
    // Replace with your camera matrices and distortion coefficients
    cv::Mat noIRCameraMatrix = (cv::Mat_<double>(3, 3) << 518.9195740674662, 0, 320.1026540670163,
                                0, 518.5081269435317, 243.2790672180749,
                                0, 0, 1);
    cv::Mat noIRDistCoeff = (cv::Mat_<double>(1, 5) << 0.1915101121545636, -0.5095826906034406, 0.0009763870143416878, 0.003755940244580556, 0.6579037599023364); // Initialize with your data

    cv::Mat visibleCameraMatrix = (cv::Mat_<double>(3, 3) << 543.7675588809527, 0, 338.9642865181752,
                                   0, 546.5569950843243, 251.2366014083449, 0, 0, 1);
    // Initialize with your data
    cv::Mat visibleCoeff = (cv::Mat_<double>(1, 5) << 0.2249142298967107, -0.7123203204239652, 0.006148106459160329, 0.005922196296597053, 0.4575903415792326); // Initialize with your data

    // Replace with paths to your stereo image pairs
    std::vector<std::string> imagesCamera1 = {"/home/pi/Onboard_VS_Streaming_RPI/Stereo Calibration and Rectification/images/image_from_camera_1.jpg"}; // Add file paths for camera 1 images
    std::vector<std::string> imagesCamera2 = {"/home/pi/Onboard_VS_Streaming_RPI/Stereo Calibration and Rectification/images/image_from_camera_2.jpg"}; // Add file paths for camera 2 images

    // Chessboard size
    cv::Size boardSize(9, 6); // Adjust to your chessboard size

    cv::Mat R1, R2, P1, P2;
    stereoCalibrateAndRectify(imagesCamera1, imagesCamera2, boardSize,
                              noIRCameraMatrix, noIRDistCoeff,
                              visibleCameraMatrix, visibleCoeff,
                              R1, R2, P1, P2);

    // The rotation matrices (R1, R2) and projection matrices (P1, P2) can now be used for rectifying images
    cv::Mat image1 = cv::imread(imagesCamera1[0]);
    cv::Mat image2 = cv::imread(imagesCamera2[0]);

    // Compute rectification maps
    cv::Mat map1x, map1y, map2x, map2y;
    cv::initUndistortRectifyMap(noIRCameraMatrix, noIRDistCoeff, R1, P1, image1.size(), CV_32FC1, map1x, map1y);
    cv::initUndistortRectifyMap(visibleCameraMatrix, visibleCoeff, R2, P2, image2.size(), CV_32FC1, map2x, map2y);

    // Rectify and stitch images
    for (size_t i = 0; i < imagesCamera1.size(); ++i)
    {
        cv::Mat img1 = cv::imread(imagesCamera1[i]);
        cv::Mat img2 = cv::imread(imagesCamera2[i]);

        // Apply rectification
        cv::Mat rectified1, rectified2;
        cv::remap(img1, rectified1, map1x, map1y, cv::INTER_LINEAR);
        cv::remap(img2, rectified2, map2x, map2y, cv::INTER_LINEAR);

        // Stitch images
        cv::Mat stitched;
        cv::hconcat(rectified1, rectified2, stitched);

        // Save or display the stitched image
        // For example, let's display it
        cv::imshow("Stitched Image", stitched);
        cv::imwrite("stichedPicture.jpg", stitched);
        cv::waitKey(0);
    }

    std::cout << "Stereo calibration, rectification, and image stitching completed." << std::endl;

    return 0;
}
