
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#define NOIR_CAMERA 0
#define VISIBLE_CAMERA 1
#define FRAME_RATE 60

// g++ -o output_file main.cpp `pkg-config --cflags --libs opencv`

int main()
{
    bool startPreview = true;
    int hours, minutes, seconds;
    cv::Mat croppedvisibleFrames, visibleFrames;
    cv::Mat previewFrame, croppedPreviewFrame;

    // Open the camera
    cv::VideoCapture cap(VISIBLE_CAMERA);

    // Check if the camera opened successfully
    if (!cap.isOpened())
    {
        std::cerr << "Error opening camera" << std::endl;
    }

    // Set the video's visibleFrames width and height
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // Create a VideoWriter object to save the video
    cv::VideoWriter videoWriter("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), FRAME_RATE, cv::Size(640, 480));

    // Check if the VideoWriter object is created successfully
    if (!videoWriter.isOpened())
    {
        std::cerr << "Error opening video writer" << std::endl;
    }

    // ------------------------------ GET USER INPUT FOR TIME DURATION ------------------------------ //
    // printf("Enter the video recording time duration in the following format ON ONE LINE --> Hour(s) Minute(s) Second(s)\r\n");
    // scanf("%d %d %d", &hours, &minutes, &seconds);
    // printf("Time duration set to : Hours [ %d ] Minutes [ %d ] Seconds [ %d ] \r\n", hours, minutes, seconds);
    // int totalSeconds = hours * 3600 + minutes * 60 + seconds;

    // "Preview" window
    printf("\n\n---------------------------- Preview window opening now ----------------------------\r\n\n");
    while (startPreview)
    {
        cap >> previewFrame;
        cv::Rect roiVisiblePreview(previewFrame.cols / 4, previewFrame.rows / 4, previewFrame.cols / 2, previewFrame.rows / 2);
        croppedPreviewFrame = previewFrame(roiVisiblePreview);
        cv::resize(croppedPreviewFrame, croppedPreviewFrame, previewFrame.size());

        // Display the visibleFrames
        // if (camIndex == 1)
        cv::moveWindow("Preview Camera Feed", 249 + 125, 246 + -20);
        cv::imshow("Preview Camera Feed", croppedPreviewFrame);
        // 32 == spacebar
        if (cv::waitKey(1) == 32)
        {
            cv::destroyWindow("Preview Camera Feed");
            startPreview = false;
            // break;
        }
    }
    startPreview = true;
    printf("\n----------------------------RECORDING VIDEO NOW----------------------------\r\n");

    // Does the actual recording
    while (startPreview)
    {
        // cv::Mat visibleFrames;

        // Capture a visibleFrames from the camera
        cap >> visibleFrames;

        // Check if the visibleFrames is empty
        if (visibleFrames.empty())
        {
            std::cerr << "Error capturing visibleFrames" << std::endl;
            break;
        }

        cv::Rect roiVisible(visibleFrames.cols / 4, visibleFrames.rows / 4, visibleFrames.cols / 2, visibleFrames.rows / 2);
        croppedvisibleFrames = visibleFrames(roiVisible);
        cv::resize(croppedvisibleFrames, croppedvisibleFrames, visibleFrames.size());

        // Display the visibleFrames
        // if (camIndex == 1)
        cv::moveWindow("Visible Camera Feed", 249 + 125, 246 + -20);

        // Only use for 'illustative purposes
        cv::imshow("Visible Camera Feed", croppedvisibleFrames);

        // Write the visibleFrames to the video file
        videoWriter.write(croppedvisibleFrames);

        // Check for the 'SPACE' key to exit the loop
        if (cv::waitKey(1) == 32)
        {
            // printf("Exiting Preview mode NOW \r\n");
            startPreview = false;
            // break;
        }
    }

    // Release the camera and video writer
    cap.release();
    videoWriter.release();

    // Close all OpenCV windows
    cv::destroyAllWindows();

    return 0;
}
