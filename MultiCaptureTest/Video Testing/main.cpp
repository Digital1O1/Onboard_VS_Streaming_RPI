
#include <opencv2/opencv.hpp>
#include <iostream>
// g++ -o output_file main.cpp `pkg-config --cflags --libs opencv`
void processVideo(int camIndex)
{
    // Open the camera
    cv::VideoCapture cap(camIndex); // 0 represents the default camera, change it if you have multiple cameras

    // Check if the camera opened successfully
    if (!cap.isOpened())
    {
        std::cerr << "Error opening camera" << std::endl;
        return;
    }

    // Set the video's frame width and height
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // Create a VideoWriter object to save the video
    cv::VideoWriter videoWriter("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(640, 480));

    // Check if the VideoWriter object is created successfully
    if (!videoWriter.isOpened())
    {
        std::cerr << "Error opening video writer" << std::endl;
        return;
    }

    // Main loop
    while (true)
    {
        cv::Mat frame;

        // Capture a frame from the camera
        cap >> frame;

        // Check if the frame is empty
        if (frame.empty())
        {
            std::cerr << "Error capturing frame" << std::endl;
            break;
        }

        // Display the frame
        // if (camIndex == 1)
        cv::imshow("Camera", frame);

        // Write the frame to the video file
        videoWriter.write(frame);

        // Check for the 'ESC' key to exit the loop
        if (cv::waitKey(1) == 27)
        {
            break;
        }
    }

    // Release the camera and video writer
    cap.release();
    videoWriter.release();

    // Close all OpenCV windows
    cv::destroyAllWindows();
}

int main()
{
    // Call the function to process video
    int camIndex;

    std::cout << "Cam index: ";
    std::cin >> camIndex;
    processVideo(camIndex);

    return 0;
}
