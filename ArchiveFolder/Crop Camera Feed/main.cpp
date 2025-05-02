#include <opencv2/opencv.hpp>

int main()
{
    // Open video stream
    cv::VideoCapture cap(1); // Replace "video.mp4" with the path to your video file or camera index

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // Check if the video stream is opened successfully
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to open the video stream." << std::endl;
        return -1;
    }

    // Define the zoom factor (e.g., zoom in by 2x)
    double zoom_factor = 2.0;

    // Main loop
    while (true)
    {
        // Read frame from the video stream
        cv::Mat frame;
        cap >> frame;

        // Check if the frame is empty (end of video)
        if (frame.empty())
            break;

        // Resize the frame to zoom in
        cv::Mat zoomed_frame;
        cv::resize(frame, zoomed_frame, cv::Size(), zoom_factor, zoom_factor);

        // Define the region of interest (ROI) to crop
        int crop_x = 100;      // x-coordinate of the top-left corner of the ROI
        int crop_y = 100;      // y-coordinate of the top-left corner of the ROI
        int crop_width = 200;  // width of the ROI
        int crop_height = 200; // height of the ROI

        // Crop the zoomed frame
        cv::Rect roi(crop_x, crop_y, crop_width, crop_height);
        cv::Mat cropped_frame = zoomed_frame(roi).clone(); // Make a copy of the cropped region

        // Display the original and cropped frames
        cv::imshow("Original Frame", frame);
        cv::imshow("Cropped Frame", cropped_frame);

        // Check for exit command
        if (cv::waitKey(1) == 27) // 'ESC' key
            break;
    }

    // Release resources
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
