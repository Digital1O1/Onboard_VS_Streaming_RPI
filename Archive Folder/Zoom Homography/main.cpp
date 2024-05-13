// https://docs.opencv.org/4.x/d9/dab/tutorial_homography.html
// #include <yaml-cpp/yaml.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>
#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1
#define CHESSBOARD_COLUMNS 9
#define CHESSBOARD_ROWS 6

int main()
{
    // Open two camera streams
    cv::VideoCapture noIR(NOIR_CAMERA);
    cv::VideoCapture visible(VISIBLE_CAMERA);
    // Check if the cameras are opened
    if (!noIR.isOpened() || !visible.isOpened())
    {
        std::cerr << "Error: Cameras not accessible" << std::endl;
        return -1;
    }

    printf("\nPress either q or Q to capture the image \r\n");
    bool previewFlag = true;
    cv::Mat irFrames, visibleFrames, combinedFrames, zoomedVisibleFrames, zoomedIrFrames;
    while (previewFlag)
    {
        noIR >> irFrames;
        visible >> visibleFrames;

        if (irFrames.empty() || visibleFrames.empty())
        {
            std::cerr << "IMAGE NOT CAPTURED " << std::endl;
        }

        // Zoom stuff

        // Define the region of interest (ROI) for zooming in (half size)
        cv::Rect roiVisible(visibleFrames.cols / 4, visibleFrames.rows / 4, visibleFrames.cols / 2, visibleFrames.rows / 2);
        cv::Rect roiIR(irFrames.cols / 4, irFrames.rows / 4, irFrames.cols / 2, irFrames.rows / 2);

        // Zoom in the visible frame
        zoomedVisibleFrames = visibleFrames(roiVisible);
        cv::resize(zoomedVisibleFrames, zoomedVisibleFrames, visibleFrames.size());

        // Zoom in the IR frame
        zoomedIrFrames = irFrames(roiIR);
        cv::resize(zoomedIrFrames, zoomedIrFrames, irFrames.size());

        // cv::Mat irFramesGray, visibleFramesGray;
        // cv::cvtColor(zoomedIrFrames, irFramesGray, cv::COLOR_BGR2GRAY);
        // cv::cvtColor(zoomedVisibleFrames, visibleFramesGray, cv::COLOR_BGR2GRAY);

        // // Combine images that were taken from cameras
        // // cv::Mat combinedFrames;
        cv::hconcat(zoomedIrFrames, zoomedVisibleFrames, combinedFrames);
        cv::imshow("Preview window", combinedFrames);

        // cv::hconcat(irFrames, visibleFrames, combinedFrames);
        // cv::imshow("Preview window", combinedFrames);

        if (cv::waitKey(1) == 'q' || cv::waitKey(1) == 'Q')
        {
            previewFlag = false;
        }
    }
    previewFlag = true;
    cv::destroyWindow("Preview window");

    // // Zoom stuff

    // // Define the region of interest (ROI) for zooming in (half size)
    // cv::Rect roiVisible(visibleFrames.cols / 4, visibleFrames.rows / 4, visibleFrames.cols / 2, visibleFrames.rows / 2);
    // cv::Rect roiIR(irFrames.cols / 4, irFrames.rows / 4, irFrames.cols / 2, irFrames.rows / 2);

    // // Zoom in the visible frame
    // cv::Mat zoomedVisibleFrames = visibleFrames(roiVisible);
    // cv::resize(zoomedVisibleFrames, zoomedVisibleFrames, visibleFrames.size());

    // // Zoom in the IR frame
    // cv::Mat zoomedIrFrames = irFrames(roiIR);
    // cv::resize(zoomedIrFrames, zoomedIrFrames, irFrames.size());

    cv::Mat irFramesGray, visibleFramesGray;
    cv::cvtColor(zoomedIrFrames, irFramesGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(zoomedVisibleFrames, visibleFramesGray, cv::COLOR_BGR2GRAY);

    // // Combine images that were taken from cameras
    // // cv::Mat combinedFrames;
    // cv::hconcat(zoomedIrFrames, zoomedVisibleFrames, combinedFrames);

    cv::Size patternsize(CHESSBOARD_COLUMNS, CHESSBOARD_ROWS); // number of corners (9x6 chessboard)
    std::vector<cv::Point2f> corners1, corners2;

    bool patternfound1 = cv::findChessboardCorners(irFrames, patternsize, corners1);
    bool patternfound2 = cv::findChessboardCorners(visibleFrames, patternsize, corners2);

    if (!patternfound1 || !patternfound2)
    {
        std::cerr << "Error: Chessboard not found in one of the images." << std::endl;
        return -1;
    }

    // Refine corner positions
    cv::cornerSubPix(irFramesGray, corners1, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
    cv::cornerSubPix(visibleFramesGray, corners2, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));

    // Compute the homography matrix to align IR --> visible
    cv::Mat infraredToVisibleHomography = cv::findHomography(corners1, corners2, cv::RANSAC);
    cv::Mat visibleToInfraredHomography = cv::findHomography(corners2, corners1, cv::RANSAC);

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

    // Write calculated Homography files to file

    // Write the matrix to the file
    saveFS << "irToVisibleHomography" << infraredToVisibleHomography;
    saveFS << "visibleToIRHomography" << visibleToInfraredHomography;

    // Release the file storage
    saveFS.release();

    std::cout << "\nIR --> VISIBLE Homography matrix:\n"
              << infraredToVisibleHomography << std::endl;

    std::cout << "\n\nVISIBLE --> IR Homography matrix:\n"
              << visibleToInfraredHomography << std::endl;

    std::cout << "Homography matricies are saved to : " << filename << std::endl;

    // Use homography to warp images (optional)
    // Warps image from IR --> Visible
    cv::Mat irWarpedFrame;
    cv::Mat visibleWarpedFrame;

    // Homography matrix being used to wrap the image from irFrame so it aligns with visibleFrame
    // Doesn't directly apply homography matrix to visible camera but to IR camera to match perspective to visible
    // wrapPerspective() applies the homography matrix to irFrames so it can match visibleFrames

    // void cv::warpPerspective	(
    // InputArray 	src,
    // OutputArray 	dst,
    // InputArray/ Mat object after using cv::findHomography 	M,
    // Size 	dsize,
    // int 	flags = INTER_LINEAR,
    // int 	borderMode = BORDER_CONSTANT,
    // const Scalar & 	borderValue = Scalar()
    // )

    // Parameters
    //  - src	        input image.
    //  - dst	        output image that has the size dsize and the same type as src .
    //  - M	3×3         transformation matrix.
    //  - dsize	        size of the output image.
    //  - flags	        combination of interpolation methods (INTER_LINEAR or INTER_NEAREST) and the optional flag WARP_INVERSE_MAP, that sets M as the inverse transformation ( dst→src ).
    //  - borderMode	pixel extrapolation method (BORDER_CONSTANT or BORDER_REPLICATE).
    //  - borderValue	value used in case of a constant border; by default, it equals 0.

    // Projects IR to VISIBLE
    cv::warpPerspective(zoomedIrFrames, irWarpedFrame, infraredToVisibleHomography, zoomedVisibleFrames.size());

    // Projects Visible to IR
    cv::warpPerspective(zoomedVisibleFrames, visibleWarpedFrame, visibleToInfraredHomography, zoomedIrFrames.size());

    cv::imshow("IR to VISIBLE", irWarpedFrame);
    cv::imshow("VISIBLE to IR", visibleWarpedFrame);
    cv::imshow("Raw images taken", combinedFrames);

    cv::imwrite("IR_to_VISIBLE.jpg", irWarpedFrame);
    cv::imwrite("VISIBLE_to_IR.jpg", visibleWarpedFrame);
    cv::imwrite("Raw_Images_Taken.jpg", combinedFrames);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
