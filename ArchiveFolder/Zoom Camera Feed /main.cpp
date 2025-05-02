#include <opencv2/opencv.hpp>

#define VIEWPORT_HEIGHT 720
#define VIEWPORT_WIDTH 1280
#define STREAM_RESOLUTION_HEIGHT 640
#define STREAM_RESOLUTION_WIDTH 480
#define UP_NUMERICAL 184
#define DOWN_NUMERICAL 178
#define LEFT_NUMERICAL 180
#define RIGHT_NUMERICAL 182
#define UP_PAD 82
#define DOWN_PAD 84
#define LEFT_PAD 81
#define RIGHT_PAD 83

/*
    Numerical keypad
    2 = 178
    6 = 182
    8 = 184
    4 = 180
*/
using namespace cv;

int main()
{
    // Open the default camera
    VideoCapture cap(1);

    // Check if the camera opened successfully
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to open the camera\n";
        return -1;
    }

    // Create a window to display the zoomed-in frame
    namedWindow("Zoomed Frame", WINDOW_NORMAL);

    // This only 'streches' out the window and not nessessary the video feed that we're intended to apply this
    // resizeWindow("Zoomed Frame", 1280, 720);

    int counter_up = 0;
    int counter_down = 0;
    int counter_left = 0;
    int counter_right = 0;

    int xCoordinate = 2;
    int yCoordinate = 2;
    int cropWidth = 2;
    int cropHeight = 2;

    // namedWindow("Regular Frame", WINDOW_NORMAL);
    //  Loop to continuously capture frames from the camera
    while (true)
    {
        int key = cv::waitKey(1);

        // std::cout << "Key pressed: " << key << std::endl;

        Mat frame;
        cap >> frame; // Capture frame from the camera

        // Check if the frame is empty
        if (frame.empty())
        {
            std::cerr << "Error: Unable to capture frame\n";
            break;
        }

        // Define the region of interest (ROI) for zooming in
        // Rect roi (x , y , crop_width , crop_height);
        Rect roi(frame.cols / xCoordinate, frame.rows / yCoordinate, frame.cols / cropWidth, frame.rows / cropHeight);
        // Rect roi(xCoordinate, yCoordinate, cropWidth, cropHeight);

        Mat zoomedFrame = frame(roi);

        // resize(zoomedFrame, zoomedFrame, frame.size());

        // To show frames independently use this line 640 x 480 --> 640 width | 480 height
        resize(zoomedFrame, zoomedFrame, Size(STREAM_RESOLUTION_HEIGHT, STREAM_RESOLUTION_WIDTH));

        // Resize the zoomed frame to the original size
        // To show side by side uncomment this line
        // resize(zoomedFrame, zoomedFrame, frame.size());

        // Show both frames next to each other
        Mat sideBySide;
        hconcat(frame, zoomedFrame, sideBySide);

        // Display the zoomed frame individually
        imshow("Zoomed Frame", zoomedFrame);
        // imshow("Regular Frame", frame);

        // Show the frames side by side
        // imshow("Zoomed Frame  Regular Frame", sideBySide);

        // Increment counters if arrow keys are pressed
        if (key == UP_PAD) // Up arrow key
        {
            xCoordinate++;
            if (xCoordinate <= 2)
            {
                xCoordinate = 2;
            }
            std::cout << "xCoordinate : " << xCoordinate << std::endl;
        }
        else if (key == DOWN_PAD) // Down arrow key
        {
            yCoordinate++;
            if (yCoordinate <= 2)
            {
                yCoordinate = 2;
            }
            std::cout << "yCoordinate : " << yCoordinate << std::endl;
        }
        else if (key == LEFT_PAD) // Left arrow key
        {
            cropWidth++;
            if (cropWidth <= 2)
            {
                cropWidth = 2;
            }
            std::cout << "cropWidth : " << cropWidth << std::endl;
        }
        else if (key == RIGHT_PAD) // Right arrow key
        {
            cropHeight++;
            if (cropHeight <= 2)
            {
                cropHeight = 2;
            }
            std::cout << "cropHeight : " << cropHeight << std::endl;
        }
        else if (key == UP_NUMERICAL) // Down arrow key
        {
            xCoordinate--;
            if (xCoordinate <= 2)
            {
                xCoordinate = 2;
            }
            std::cout << "xCoordinate : " << xCoordinate << std::endl;
        }
        else if (key == DOWN_NUMERICAL) // Left arrow key
        {
            yCoordinate--;
            if (yCoordinate <= 2)
            {
                yCoordinate = 2;
            }
            std::cout << "yCoordinate : " << yCoordinate << std::endl;
        }
        else if (key == LEFT_NUMERICAL) // Right arrow key
        {
            cropWidth--;
            if (cropWidth <= 2)
            {
                cropWidth = 2;
            }
            std::cout << "cropWidth : " << cropWidth << std::endl;
        }
        else if (key == RIGHT_NUMERICAL) // Right arrow key
        {
            cropHeight--;
            if (cropHeight <= 2)
            {
                cropHeight = 2;
            }
            std::cout << "cropHeight : " << cropHeight << std::endl;
        }
        // Check for user to press ESC key to exit program
        if (key == 27)
        {
            std::cout << "Exiting...\n";
            break;
        }
    }

    // Release the camera and destroy windows
    cap.release();
    destroyAllWindows();

    return 0;
}
