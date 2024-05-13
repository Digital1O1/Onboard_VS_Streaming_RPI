#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>

#define PORT 12345
// Start this program first
int main()
{
    printf("Raspberry PI program started\r\n\n\n");
    // Initialize the camera
    cv::VideoCapture cap("/dev/video0", cv::CAP_V4L2);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Unable to open camera." << std::endl;
        return -1;
    }

    // Set the capture format to RAW
    // cap.set(CAP_PROP_FOURCC, V4L2_PIX_FMT_SRGGB10);
    // cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('R', 'G', '1', '0'));

    // Set the camera resolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    // Open a socket
    auto openSocketStart = std::chrono::high_resolution_clock::now();

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cerr << "Error: Unable to create socket." << std::endl;
        return -1;
    }
    else
    {
        auto openSocketStop = std::chrono::high_resolution_clock::now();
        auto socketDuration = std::chrono::duration_cast<std::chrono::microseconds>(openSocketStop - openSocketStart);
        std::cout << "Time elapsed for RPI to CREATE SOCKET : " << socketDuration.count() << " microseconds" << std::endl;
    }

    auto bindSocketStart = std::chrono::high_resolution_clock::now();

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Error: Unable to bind socket." << std::endl;
        close(server_socket);
        return -1;
    }
    else
    {
        auto bindSocketStop = std::chrono::high_resolution_clock::now();
        auto bindSocketDuration = std::chrono::duration_cast<std::chrono::microseconds>(bindSocketStop - bindSocketStart);
        std::cout << "Time elapsed for RPI to BIND SOCKET : " << bindSocketDuration.count() << " microseconds" << std::endl;
    }
    listen(server_socket, 1);
    std::cout << "Waiting for connection..." << std::endl;

    // Accept a connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1)
    {
        std::cerr << "Error: Unable to accept connection." << std::endl;
        close(server_socket);
        return -1;
    }

    std::cout << "Connection established." << std::endl;

    auto captureFrameStart = std::chrono::high_resolution_clock::now();

    // Capture a single image
    cv::Mat frame;
    cap >> frame;

    auto captureFrameStop = std::chrono::high_resolution_clock::now();
    auto captureDuration = std::chrono::duration_cast<std::chrono::microseconds>(captureFrameStop - captureFrameStart);
    std::cout << "Time elapsed for RPI to BIND SOCKET : " << captureDuration.count() << " microseconds" << std::endl;

    // Send resolution information
    auto sendImageDataStart = std::chrono::high_resolution_clock::now();

    int resolution[2] = {frame.cols, frame.rows};
    // std::cout << resolution[0] << " " << resolution[1] << std::endl;
    send(client_socket, resolution, sizeof(resolution), 0);

    // Send the image data
    int imgSize = frame.total() * frame.elemSize();
    if (send(client_socket, frame.data, imgSize, 0) == -1)
    {
        std::cerr << "Error: Unable to send image data." << std::endl;
    }
    else
    {
        auto sendImageDataEnd = std::chrono::high_resolution_clock::now();
        auto sendImageDataDuration = std::chrono::duration_cast<std::chrono::microseconds>(sendImageDataEnd - sendImageDataStart);
        std::cout << "Time elapsed for RPI to SEND RAW IMAGE TO WINDOWS : " << sendImageDataDuration.count() << " microseconds" << std::endl;
    }
    // Receive resolution information
    auto recieveResolutionStart = std::chrono::high_resolution_clock::now();

    int threshold_resolution[2];
    if (recv(client_socket, threshold_resolution, sizeof(threshold_resolution), 0) == -1)
    {
        std::cerr << "Error: Unable to receive threshold_resolution information." << std::endl;
        close(client_socket);
        close(server_socket);
        return -1;
    }
    else
    {
        auto recieveResolutionEnd = std::chrono::high_resolution_clock::now();
        auto recieveResolutionDuration = std::chrono::duration_cast<std::chrono::microseconds>(recieveResolutionEnd - recieveResolutionStart);
        std::cout << "Time elapsed for RPI to RECIEVE THRESHOLD RESOLUTION FROM WINDOWS : " << recieveResolutionDuration.count() << " microseconds" << std::endl;
    }
    int imgWidth = threshold_resolution[0];
    int imgHeight = threshold_resolution[1];
    std::cout << "Received Resolution: " << imgWidth << " x " << imgHeight << std::endl;

    // Allocate memory for the thresholded image

    cv::Mat receivedThresholdedImage(imgHeight, imgWidth, CV_8UC1);

    // Receive the thresholded image data
    auto thresholdImageStart = std::chrono::high_resolution_clock::now();

    int thresholdedImgSize = imgWidth * imgHeight;
    char *thresholdedBuffer = new char[thresholdedImgSize];
    int thresholdedBytesReceived = 0;

    while (thresholdedBytesReceived < thresholdedImgSize)
    {
        int result = recv(client_socket, thresholdedBuffer + thresholdedBytesReceived, thresholdedImgSize - thresholdedBytesReceived, 0);
        if (result == -1 || result == 0)
        {
            std::cerr << "Error: Unable to receive thresholded image data." << std::endl;
            delete[] thresholdedBuffer;
            close(client_socket);
            close(server_socket);
            return -1;
        }
        thresholdedBytesReceived += result;
    }

    // Create a Mat object from the received thresholded data
    memcpy(receivedThresholdedImage.data, thresholdedBuffer, thresholdedImgSize);

    auto thresholdImageEnd = std::chrono::high_resolution_clock::now();
    auto thresholdImageDuration = std::chrono::duration_cast<std::chrono::microseconds>(thresholdImageEnd - thresholdImageStart);
    std::cout << "Time elapsed for RPI to RECIEVE THRESHOLDED IMAGE DATA FROM WINDOWS : " << thresholdImageDuration.count() << " microseconds" << std::endl;

    // // Display the received thresholded image on Raspberry Pi for troubleshooting
    // cv::imshow("Raspberry Pi - Raw Image", frame);
    // cv::imshow("Raspberry Pi - Thresholded Image from WINDOWS", receivedThresholdedImage);
    // cv::waitKey(0);

    // Display the captured image on Raspberry Pi
    // cv::imshow("Raspberry Pi - Captured Image", frame);
    // cv::waitKey(0);

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
