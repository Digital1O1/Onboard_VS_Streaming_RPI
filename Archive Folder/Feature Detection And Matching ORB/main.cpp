#include <opencv2/opencv.hpp>
#include <vector>
#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1
int main()
{
    // Open two camera streams
    cv::VideoCapture cap1(NOIR_CAMERA);    // Change 0 to the appropriate camera index
    cv::VideoCapture cap2(VISIBLE_CAMERA); // Change 1 to the appropriate camera index

    // Check if the cameras are opened
    if (!cap1.isOpened() || !cap2.isOpened())
    {
        std::cerr << "Error: Cameras not accessible" << std::endl;
        return -1;
    }

    printf("\nPress either q or Q to capture the image \r\n");
    bool previewFlag = true;
    cv::Mat irFrames, visibleFrames, combinedFrames;
    while (previewFlag)
    {
        cap1 >> irFrames;
        cap2 >> visibleFrames;

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

    // Convert images to grayscale
    cv::Mat gray1, gray2;
    cv::cvtColor(irFrames, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(visibleFrames, gray2, cv::COLOR_BGR2GRAY);

    // Detect ORB keypoints and descriptors
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(gray1, cv::noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(gray2, cv::noArray(), keypoints2, descriptors2);

    // Match descriptors using BFMatcher
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Sort matches by their distance
    std::sort(matches.begin(), matches.end());

    // Remove not so good matches
    const int numGoodMatches = matches.size() * 0.15;
    matches.erase(matches.begin() + numGoodMatches, matches.end());

    // Draw top matches
    cv::Mat imgMatches;
    cv::drawMatches(irFrames, keypoints1, visibleFrames, keypoints2, matches, imgMatches);

    // Extract location of good matches
    std::vector<cv::Point2f> points1, points2;

    for (size_t i = 0; i < matches.size(); i++)
    {
        points1.push_back(keypoints1[matches[i].queryIdx].pt);
        points2.push_back(keypoints2[matches[i].trainIdx].pt);
    }

    // Find homography
    cv::Mat calculatedHomography = cv::findHomography(points1, points2, cv::RANSAC);

    // Use homography to warp image
    cv::Mat img1Warped;
    cv::warpPerspective(irFrames, img1Warped, calculatedHomography, visibleFrames.size());

    // Display the warped image
    cv::imshow("Warped Image", img1Warped);
    cv::imshow("Matches", imgMatches);

    cv::imwrite("warpedImage.jpg", img1Warped);
    cv::imwrite("matches.jpg", imgMatches);

    std::cout << "Calculated Homography\n\n"
              << calculatedHomography << std::endl;

    cv::waitKey(0);

    return 0;
}
