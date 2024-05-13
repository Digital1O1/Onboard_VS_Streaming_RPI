#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <chrono>
#include <cstdint>
#include <inttypes.h>

#define IR_CAMERA 0
#define VISIBLE_CAMERA 1
#define MICROSECOND_CONVERSION 1000000
#define MILLISECOND_CONVERSION 1000
#define IMAGE_DATA_BUFFER 65536
// #define THRESHOLD_SAVE_PATH "/home/pi/Desktop/Onboard_VS_Streaming_RPI/Send_Picture_RPI/build"
const char *TRESHOLD_FILENAME = "threshold_image.jpg";
const char *TRESHOLD_FILE_SAVE_PATH = "/home/pi/Onboard_VS_Streaming_RPI/Send_Picture_RPI/build/threshold_image.jpg";

// const char *SERVER_IP = "172.17.140.231"; // Replace with the Windows machine's IP
const char *SERVER_IP = "192.168.1.177";

const int PORT = 1025; // Replace with the port you want to use

int main()
{
    // -------------------------- [ Client side START ] -------------------------- //
    printf("\nRASPBERRY PI Program started \r\n");

    // -------------------------- [ Create LINUX SOCKET ] -------------------------- //
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Socket creation failed");
        return 1;
    }
    // else
    //     printf("Socket created to Windows machine\r\nConnecting to port [ %d ] \r\nConnecting to WINDOWS IP address [ %s ]\r\n", PORT, SERVER_IP);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // -------------------------- [ Connect to WINDOWS SERVER ] -------------------------- //
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Connection to the server failed");
        return 1;
    }
    // else
    //     printf("Connection to server established\r\n");

    // -------------------------- [ Start video capture ] -------------------------- //
    cv::VideoCapture cap(IR_CAMERA); // Use the default camera (0) or adjust as needed

    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera." << std::endl;
        return 1;
    }

    // auto capture_frame_START = std::chrono::high_resolution_clock::now();
    // // cam0_frame --> demosaic Mat
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

    // Save the cam0_frame as a TIFF file
    cv::imwrite("raw_image.tiff", cam0_frame);
    cv::Mat raw_image = cv::imread("raw_image.tiff", cv::IMREAD_UNCHANGED);

    if (raw_image.empty())
    {
        std::cerr << "Error: Could not read the raw image." << std::endl;
        return 1;
    }
    // cv::imshow("Raw/TIFF image", raw_image);
    // cv::waitKey(0);

    // Send image size to the Windows machine
    int rawImageWidth = raw_image.cols;
    int rawImageHeight = raw_image.rows;

    if (send(clientSocket, &rawImageWidth, sizeof(rawImageWidth), 0) == -1 ||
        send(clientSocket, &rawImageHeight, sizeof(rawImageHeight), 0) == -1)
    {
        perror("Sending image size failed");
        return 1;
    }
    else
    {
        printf("Image size of [ %d ] x [ %d ] sent to windows machine\r\n", rawImageWidth, rawImageHeight);
    }

    // Send raw pixel data to the Windows machine
    // auto send_Image_START = std::chrono::high_resolution_clock::now();

    if (send(clientSocket, raw_image.data, rawImageWidth * rawImageHeight, 0) == -1)
    {
        perror("Sending image data failed");
        return 1;
    }
    else
    {
        // auto send_Image_END = std::chrono::high_resolution_clock::now();
        // auto send_Image_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(send_Image_END - send_Image_START);
        // printf("Time spent to send image DATA: [%" PRId64 " ] or %lf 10^-9 SECONDS \r\n\nNumber of bytes sent: [ %d ] out of [ %d ] to Windows machine sent SUCCESSFULLY\r\n", send_Image_DURATION.count(), static_cast<double>(send_Image_DURATION.count()) / MICROSECOND_CONVERSION, imageWidth * rawImageHeight, imageWidth * rawImageHeight);
        printf("Number of bytes sent to Windows machine : [ %d ] \r\n", rawImageWidth * rawImageHeight);
    }

    // Receive thresholded image size from Windows machine
    // ssize_t recv(int __fd, void *__buf, size_t __n, int __flags)

    int thresholdedWidth, thresholdedHeight;
    if (recv(clientSocket, &thresholdedWidth, sizeof(thresholdedWidth), 0) == -1 ||
        recv(clientSocket, &thresholdedHeight, sizeof(thresholdedHeight), 0) == -1)
    {
        perror("Error receiving thresholded image size");
        return 1;
    }

    printf("Received thresholded image size: [ %d ] x [ %d ] \r\n", thresholdedWidth, thresholdedHeight);

    // Receive thresholded pixel data
    char thresholdedImageBuffer[IMAGE_DATA_BUFFER];
    int totalThresholdedBytesReceived = 0;
    int remainingThresholdedBytes = thresholdedWidth * thresholdedHeight;

    // OpenCV Mat for thresholded image
    cv::Mat thresholdedImage(thresholdedHeight, thresholdedWidth, CV_8UC1);

    while (totalThresholdedBytesReceived < remainingThresholdedBytes)
    {
        int receivedThresholdedBytes = recv(clientSocket, thresholdedImageBuffer, sizeof(thresholdedImageBuffer), 0);
        if (receivedThresholdedBytes == -1)
        {
            perror("Failed to receive thresholded image data");
            break;
        }

        std::memcpy(thresholdedImage.data + totalThresholdedBytesReceived, thresholdedImageBuffer, receivedThresholdedBytes);
        totalThresholdedBytesReceived += receivedThresholdedBytes;
    }

    // Display the thresholded image on the Raspberry Pi
    cv::imshow("Recieved Thresholded Image RPI ", thresholdedImage);
    cv::waitKey(0);

    return 0;
}
