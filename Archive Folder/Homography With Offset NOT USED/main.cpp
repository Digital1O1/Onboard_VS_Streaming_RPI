// https://docs.opencv.org/4.x/d9/dab/tutorial_homography.html

#include <opencv2/opencv.hpp>
#include <vector>
#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1

void calculateWarpedImageSize(const cv::Mat &srcImage, const cv::Mat &H, cv::Size &size, cv::Point &offset)
{
    std::vector<cv::Point2f> corners(4);
    corners[0] = cv::Point2f(0, 0);
    corners[1] = cv::Point2f(srcImage.cols, 0);
    corners[2] = cv::Point2f(srcImage.cols, srcImage.rows);
    corners[3] = cv::Point2f(0, srcImage.rows);

    // Transform the corners using the homography
    std::vector<cv::Point2f> warpedCorners(4);
    cv::perspectiveTransform(corners, warpedCorners, H);

    // Find min and max x and y of warped corners
    float minX = std::min({warpedCorners[0].x, warpedCorners[1].x, warpedCorners[2].x, warpedCorners[3].x});
    float maxX = std::max({warpedCorners[0].x, warpedCorners[1].x, warpedCorners[2].x, warpedCorners[3].x});
    float minY = std::min({warpedCorners[0].y, warpedCorners[1].y, warpedCorners[2].y, warpedCorners[3].y});
    float maxY = std::max({warpedCorners[0].y, warpedCorners[1].y, warpedCorners[2].y, warpedCorners[3].y});

    // Calculate size and offset
    size = cv::Size(static_cast<int>(std::ceil(maxX - minX)), static_cast<int>(std::ceil(maxY - minY)));
    offset = cv::Point(static_cast<int>(std::floor(minX)), static_cast<int>(std::floor(minY)));
}

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
    cv::Mat irFrames, visibleFrames, combinedFrames;
    while (previewFlag)
    {
        noIR >> irFrames;
        visible >> visibleFrames;

        if (irFrames.empty() || visibleFrames.empty())
        {
            std::cerr << "IMAGE NOT CAPTURED " << std::endl;
        }

        cv::hconcat(irFrames, visibleFrames, combinedFrames);

        cv::imshow("Preview window", combinedFrames);

        if (cv::waitKey(1) == 'q' || cv::waitKey(1) == 'Q')
        {
            previewFlag = false;
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

    cv::Size patternsize(9, 6); // number of corners (9x6 chessboard)
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

    std::cout << "IR --> VISIBLE Homography matrix:\n"
              << infraredToVisibleHomography << std::endl;

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

    // cv::Size newSize;
    // cv::Point offset;
    // calculateWarpedImageSize(irFrames, calculatedHomography, newSize, offset);

    // Projects IR to VISIBLE
    cv::warpPerspective(irFrames, irWarpedFrame, infraredToVisibleHomography, visibleFrames.size());

    // Projects Visible to IR
    cv::warpPerspective(visibleFrames, visibleWarpedFrame, visibleToInfraredHomography, irFrames.size());

    cv::imshow("IR Warped Image", irWarpedFrame);
    cv::imshow("VISIBLE Warped Image", visibleWarpedFrame);
    cv::imshow("Raw images taken", combinedFrames);

    cv::imwrite("IRwarpedImage.jpg", irWarpedFrame);
    cv::imwrite("VisibleWarpedImage.jpg", visibleWarpedFrame);
    cv::imwrite("combinedImages.jpg", combinedFrames);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
