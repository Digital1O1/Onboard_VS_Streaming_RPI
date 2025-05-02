#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture cap(0); // Use the default camera (0) or adjust as needed

    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera." << std::endl;
        return 1;
    }
    else
    {
        cap.set(cv::CAP_PROP_CONVERT_RGB, false);

        // Print out camera properties
        std::cout << "Camera Properties:" << std::endl;
        std::cout << "-------------------" << std::endl;
        std::cout << "Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
        std::cout << "Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
        std::cout << "FPS: " << cap.get(cv::CAP_PROP_FPS) << std::endl;
        std::cout << "Format: " << cap.get(cv::CAP_PROP_FORMAT) << std::endl;
        std::cout << "Mode: " << cap.get(cv::CAP_PROP_MODE) << std::endl;
    }

    cv::Mat cam0_frame;
    cap >> cam0_frame;

    if (cam0_frame.empty())
    {
        std::cerr << "Error: Frame is empty." << std::endl;
        return 1;
    }

    while (true)
    {
        int key = cv::waitKey(1);

        if (key == 'Q' || key == 'q')
        {
            cap.set(cv::CAP_PROP_CONVERT_RGB, false);
            break; // Exit the loop and capture the image
        }
        cap >> cam0_frame;
        if (cam0_frame.empty())
        {
            std::cerr << "Error: Frame is empty." << std::endl;
            return 1;
        }

        // Display the preview window
        cv::imshow("Preview", cam0_frame);
    }

    // Check the number of channels
    if (cam0_frame.channels() > 1)
    {
        // Convert to single channel
        cv::cvtColor(cam0_frame, cam0_frame, cv::COLOR_BGR2GRAY);
    }

    // Demosaicing using OpenCV
    cv::Mat demosaicedFrame;
    cv::cvtColor(cam0_frame, demosaicedFrame, cv::COLOR_BayerBG2BGR); // Change the pattern if needed

    // Display the original and demosaiced frames
    cv::imshow("Bayer Frame", cam0_frame);
    cv::imshow("Demosaiced Frame", demosaicedFrame);

    cv::waitKey(0);
    return 0;
}
