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

    // GStreamer pipelines
    std::string visibleCameraPipeline = R"(
        libcamerasrc camera-name="/base/soc/i2c0mux/i2c@1/imx219@10" ! 
        video/x-raw,width=640,height=480,framerate=30/1 ! 
        videoconvert ! 
        video/x-raw,format=(string)BGR ! 
        queue ! 
        appsink
    )";

    std::string irCameraPipeline = R"(
        libcamerasrc camera-name="/base/soc/i2c0mux/i2c@0/imx219@10" ! 
        video/x-raw,width=640,height=480,framerate=30/1 ! 
        videoconvert ! 
        video/x-raw,format=(string)BGR ! 
        queue ! 
        appsink
    )";
    // Open two camera streams
    cv::VideoCapture noIR(irCameraPipeline, cv::CAP_GSTREAMER);
    cv::VideoCapture visible(visibleCameraPipeline, cv::CAP_GSTREAMER);
    // Check if the cameras are opened
    if (!noIR.isOpened() || !visible.isOpened())
    {
        std::cerr << "Error: Cameras not accessible" << std::endl;
        return -1;
    }

    printf("\nClick on the active OpenCV window and press either q or Q to capture the image \r\n");
    bool previewFlag = true;
    cv::Mat irFrames, visibleFrames, combinedFrames;
    while (previewFlag)
    {
        noIR >> irFrames;
        visible >> visibleFrames;

        if (irFrames.empty() || visibleFrames.empty())
        {
            std::cerr << "IMAGE NOT CAPTURED " << std::endl;
        }

        // Needed for IR camera
        // cv::flip(irFrames, irFrames, 0);

        cv::hconcat(irFrames, visibleFrames, combinedFrames);

        cv::imshow("Preview window", combinedFrames);

        char key = cv::waitKey(1);

        if (key == 'q' || key == 'Q')
        {
            previewFlag = false;
            printf("Exiting preview mode....\r\n");
        }
    }
    previewFlag = true;
    cv::destroyWindow("Preview window");

    cv::Mat irFramesGray, visibleFramesGray;
    cv::cvtColor(irFrames, irFramesGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(visibleFrames, visibleFramesGray, cv::COLOR_BGR2GRAY);

    // Combine images that were taken from cameras
    cv::Mat combinedImages;
    cv::hconcat(irFrames, visibleFrames, combinedFrames);

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

    std::cout << "\nIR --> VISIBLE Homography matrix:\n"
              << infraredToVisibleHomography << std::endl;

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

    std::cout << "\nHomography matricies are saved to : " << filename << std::endl;

    // Release the file storage
    saveFS.release();

    std::cout << "\n\nVISIBLE --> IR Homography matrix:\n"
              << visibleToInfraredHomography << std::endl;

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
    cv::warpPerspective(irFrames, irWarpedFrame, infraredToVisibleHomography, visibleFrames.size());

    // Projects Visible to IR | This is what's implemented in the CVG
    cv::warpPerspective(visibleFrames, visibleWarpedFrame, visibleToInfraredHomography, irFrames.size());

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
