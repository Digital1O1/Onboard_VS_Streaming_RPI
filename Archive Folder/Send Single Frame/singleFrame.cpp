#include <opencv2/opencv.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdint>
#include <inttypes.h>
#include <chrono>

#define IR_CAMERA 0
#define MICROSECOND_CONVERSION 1000000
#define MILLISECOND_CONVERSION 1000
#define IMAGE_DATA_BUFFER 65536

const char *SERVER_IP = "172.17.140.231";
const int PORT = 1025;

int main()
{
    printf("\nRASPBERRY PI Program started \r\n");

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Socket creation failed");
        close(clientSocket);
        return 1;
    }
    else
        printf("Socket created to Windows machine\r\nConnecting to port [ %d ] \r\nConnecting to WINDOWS IP address [ %s ]\r\n", PORT, SERVER_IP);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    auto connection_To_Windows_START = std::chrono::high_resolution_clock::now();
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Connection to the server failed");
        close(clientSocket);
        return 1;
    }
    else
    {
        auto connection_To_Windows_END = std::chrono::high_resolution_clock::now();
        auto connection_To_Windows_START_Duration = std::chrono::duration_cast<std::chrono::microseconds>(connection_To_Windows_END - connection_To_Windows_START);
        printf("\nTime duration to CONNECT SOCKET to WINDOWS machine : %" PRId64 " MICROSECONDS\r\n", connection_To_Windows_START_Duration.count());
    }

    cv::VideoCapture cap(IR_CAMERA);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open camera." << std::endl;
        close(clientSocket);
        return 1;
    }

    auto capture_Image_START = std::chrono::high_resolution_clock::now();
    cv::Mat cam0_frame;
    cap >> cam0_frame;
    auto capture_Image_END = std::chrono::high_resolution_clock::now();
    auto capture_Image_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(capture_Image_END - capture_Image_START);
    printf("\nTime duration to CAPTURE ONE FRAME : %" PRId64 " MICROSECONDS\r\n", capture_Image_DURATION.count());
    // cv::imshow("Original image", cam0_frame);
    // cv::waitKey(0);

    if (cam0_frame.empty())
    {
        std::cerr << "Error: Frame is empty." << std::endl;
    }
    int rawImageWidth = cam0_frame.cols;
    int rawImageHeight = cam0_frame.rows;

    // std::cout << "Captured Image Dimensions - Width: " << cam0_frame.cols << ", Height: " << cam0_frame.rows << std::endl;

    auto send_Image_Size_START = std::chrono::high_resolution_clock::now();
    if (send(clientSocket, &rawImageWidth, sizeof(rawImageWidth), 0) == -1 ||
        send(clientSocket, &rawImageHeight, sizeof(rawImageHeight), 0) == -1)
    {
        perror("Sending image size failed");
        close(clientSocket);
        return 1;
    }
    else
    {
        auto send_Image_Size_END = std::chrono::high_resolution_clock::now();
        auto send_Image_Size_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(send_Image_Size_END - send_Image_Size_START);
        printf("\nTime duration to send raw image SIZE to WINDOWS SERVER : %" PRId64 " MICROSECONDS\r\n", send_Image_Size_DURATION.count());
    }

    auto send_Image_Data_START = std::chrono::high_resolution_clock::now();
    if (send(clientSocket, cam0_frame.data, rawImageWidth * rawImageHeight, 0) == -1)
    {
        perror("Sending image data failed");
        close(clientSocket);
        return 1;
    }
    else
    {
        auto send_Image_Data_END = std::chrono::high_resolution_clock::now();
        auto send_Image_Data_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(send_Image_Data_END - send_Image_Data_START);
        printf("\nTime duration to send raw image DATA to WINDOWS SERVER : %" PRId64 " MICROSECONDS\r\n", send_Image_Data_DURATION.count());
    }

    auto recieve_Threshold_Image_Size_START = std::chrono::high_resolution_clock::now();
    int thresholdedWidth, thresholdedHeight;
    if (recv(clientSocket, &thresholdedWidth, sizeof(thresholdedWidth), 0) == -1 ||
        recv(clientSocket, &thresholdedHeight, sizeof(thresholdedHeight), 0) == -1)
    {
        perror("Error receiving thresholded image size");
        close(clientSocket);
        return 1;
    }
    else
    {
        auto recieve_Threshold_Image_Size_END = std::chrono::high_resolution_clock::now();
        auto recieve_Threshold_Image_Size_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(recieve_Threshold_Image_Size_END - recieve_Threshold_Image_Size_START);
        printf("\nTime duration to recieve thresholded image SIZE from windows machine : %" PRId64 " MICROSECONDS\r\n", recieve_Threshold_Image_Size_DURATION.count());
    }

    char thresholdedImageBuffer[IMAGE_DATA_BUFFER];
    int totalThresholdedBytesReceived = 0;
    int remainingThresholdedBytes = thresholdedWidth * thresholdedHeight;

    auto recieve_Threshold_Image_Data_START = std::chrono::high_resolution_clock::now();
    cv::Mat thresholdedImage(thresholdedHeight, thresholdedWidth, CV_8UC1);

    // Recieve threshold data
    while (totalThresholdedBytesReceived < remainingThresholdedBytes)
    {
        int receivedThresholdedBytes = recv(clientSocket, thresholdedImageBuffer, sizeof(thresholdedImageBuffer), 0);
        if (receivedThresholdedBytes == -1)
        {
            perror("Failed to receive thresholded image data");
            return 1;
        }

        std::memcpy(thresholdedImage.data + totalThresholdedBytesReceived, thresholdedImageBuffer, receivedThresholdedBytes);
        totalThresholdedBytesReceived += receivedThresholdedBytes;
    }

    auto recieve_Threshold_Image_Data_END = std::chrono::high_resolution_clock::now();
    auto recieve_Threshold_Image_Data_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(recieve_Threshold_Image_Data_END - recieve_Threshold_Image_Data_START);
    printf("\nTime duration to recieve thresholded image DATA from windows machine : %" PRId64 " MICROSECONDS\r\n", recieve_Threshold_Image_Data_DURATION.count());

    // Display the received images
    // cv::imshow("Original Image RPI", cam0_frame);
    // cv::imshow("Received Thresholded Image RPI", thresholdedImage);
    // cv::waitKey(0);

    close(clientSocket);

    return 0;
}
