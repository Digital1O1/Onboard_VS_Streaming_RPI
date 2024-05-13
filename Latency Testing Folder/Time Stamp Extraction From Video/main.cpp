#include <opencv2/opencv.hpp>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;

int main()
{
    cv::VideoCapture cap("/home/utsw-bmen-laptop/Coding Folder/Onboard_VS_Streaming_LINUX/Time Stamp Extraction/DSC_0035.MOV");
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file\n";
        return -1;
    }
    else
        printf("Video loaded\r\n");

    double fps = cap.get(cv::CAP_PROP_FPS);
    int frameCount = cap.get(cv::CAP_PROP_FRAME_COUNT);

    printf("FPS detected : %f \t Frame count : %d \r\n", fps, frameCount);

    // Create a directory to save the time_stamp_frames
    std::string timeStampFrames = "time_stamp_frames/";

    // Check if the directory already exists
    if (!fs::exists(timeStampFrames))
    {
        // Create the directory
        if (fs::create_directory(timeStampFrames))
        {
            std::cout << "Directory created successfully : " << timeStampFrames << std::endl;
        }
        else
        {
            std::cerr << "Failed to create directory: " << timeStampFrames << std::endl;
        }
    }
    else
    {
        std::cerr << "Directory already exists: " << timeStampFrames << std::endl;
    }

    for (int frameNumber = 0; frameNumber < frameCount; ++frameNumber)
    {
        double timestamp = frameNumber / fps;
        std::cout << "Frame " << frameNumber << " Timestamp : " << timestamp << " seconds\n";

        // Set the frame position to read the current frame
        cap.set(cv::CAP_PROP_POS_FRAMES, frameNumber);

        // Read the current frame
        cv::Mat frame;
        if (!cap.read(frame))
        {
            std::cerr << "Error reading frame " << frameNumber << std::endl;
            break;
        }

        // Save the frame with timestamp in the filename
        std::ostringstream filename;
        filename << timeStampFrames << "frame_" << std::fixed << std::setprecision(3) << timestamp << ".jpg"; // Or use .png if you prefer
        cv::imwrite(filename.str(), frame);
    }

    cap.release();
    return 0;
}
