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

const char *SERVER_IP = "172.17.140.231"; // Replace with the Windows machine's IP
// const char *SERVER_IP = "192.168.1.177";

const int PORT = 1025; // Replace with the port you want to use

int main()
{
    // -------------------------- [ Client side START ] -------------------------- //
    // printf("\nRASPBERRY PI Program started \r\n");

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
    // else
    //     printf("Camera stream started\r\n");

    auto capture_frame_START = std::chrono::high_resolution_clock::now();
    cv::Mat cam0_frame;
    cap >> cam0_frame;

    if (cam0_frame.empty())
    {
        std::cerr << "Error: Frame is empty." << std::endl;
        return 1;
    }

    auto capture_frame_END = std::chrono::high_resolution_clock::now();
    auto capture_duration = std::chrono::duration_cast<std::chrono::milliseconds>(capture_frame_END - capture_frame_START);
    printf("\nAmount of time to CAPTURE single image : %" PRId64 " MILLISECONDS or %lf 10^-3 SECONDS\r\n", capture_duration.count(), static_cast<double>(capture_duration.count()) / MILLISECOND_CONVERSION);

    // -------------------------- [ Encode captured frame into JPEG to transfer to WINDOWS machine ] -------------------------- //
    std::vector<uchar> raw_Image_Buffer;
    cv::imencode(".jpg", cam0_frame, raw_Image_Buffer);

    // Calculate the image size
    int imageSize = raw_Image_Buffer.size();
    auto send_Image_Size_START = std::chrono::high_resolution_clock::now();

    // -------------------------- [ Send JPEG SIZE to WINDOWS machine ] -------------------------- //
    if (send(clientSocket, &imageSize, sizeof(imageSize), 0) == -1)
    {
        perror("Sending image size failed");
        return 1;
    }
    else
    {
        auto send_Image_Size_END = std::chrono::high_resolution_clock::now();
        auto send_Image_Size_Duration = std::chrono::duration_cast<std::chrono::microseconds>(send_Image_Size_END - send_Image_Size_START);
        printf("\nTime spent to send image SIZE to Windows machine: % " PRId64 " MICROSECONDS or %lf 10^-9 SECONDS\r\n\n", send_Image_Size_Duration.count(), static_cast<double>(send_Image_Size_Duration.count()) / MICROSECOND_CONVERSION);
        // printf("\nTime spent to send image size to Windows machine: % " PRId64 " MICROSECONDS or %lf 10^-9 SECONDS\r\n\n", send_Image_Size_Duration.count(), static_cast<double>(send_Image_Size_Duration.count()) / MICROSECOND_CONVERSION);
    }
    // -------------------------- [ Send actual JPEG to WINDOWS machine ] -------------------------- //

    auto send_Image_START = std::chrono::high_resolution_clock::now();
    if (send(clientSocket, raw_Image_Buffer.data(), imageSize, 0) == -1)
    {
        perror("Sending image data failed");
        return 1;
    }
    else
    {
        auto send_Image_END = std::chrono::high_resolution_clock::now();
        auto send_Image_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(send_Image_END - send_Image_Size_START);
        // printf("Time duration during picture SIZE transmission : [ %lf ] SECONDS \r\nNumber of bytes sent : [ %zd ] out of [ %zd ]  to Windows machine sent SUCCESSFULLY\r\n", static_cast<double>(send_Image_DURATION.count()) / MICROSECOND_CONVERSION, raw_Image_Buffer.size(), imageSize);

        printf("Time spent to send image DATA : [%" PRId64 " ] or %lf 10^-9 SECONDS \r\n\nNumber of bytes sent : [ %zd ] out of [ %zd ]  to Windows machine sent SUCCESSFULLY\r\n", send_Image_DURATION.count(), static_cast<double>(send_Image_DURATION.count()) / MICROSECOND_CONVERSION, raw_Image_Buffer.size(), imageSize);
    }

    // -------------------------- [ Create save path for thresholded image ] -------------------------- //
    std::ofstream incoming_Threshold_Image(TRESHOLD_FILENAME, std::ios::binary);
    if (!incoming_Threshold_Image)
    {
        std::cerr << "Failed to create output file" << std::endl;
        return 1;
    }

    // -------------------------- [ Recieve thresholded image SIZE from WINDOWS machine ] -------------------------- //
    uint32_t imageSize_Server = 0;
    if (recv(clientSocket, &imageSize_Server, sizeof(imageSize_Server), 0) == -1)
    {
        perror("Receiving image size failed");
        close(clientSocket);
        return 1;
    }
    // else
    // {
    //     printf("Number of picture bytes received from TRESHOLDED IMAGE : [ %u ] \r\n", static_cast<uint32_t>(imageSize_Server));
    // }

    // // -------------------------- [ Create save path for thresholded image ] -------------------------- //
    // std::ofstream incoming_Threshold_Image(TRESHOLD_FILENAME, std::ios::binary);
    // if (!incoming_Threshold_Image)
    // {
    //     std::cerr << "Failed to create output file" << std::endl;
    //     return 1;
    // }

    // -------------------------- [ Create save path for thresholded image ] -------------------------- //
    char threshold_image_buffer[imageSize_Server];
    int bytesReceived = 0;

    // -------------------------- [ Recieve thresholded image size from WINDOWS machine ] -------------------------- //

    uint32_t totalBytesReceived = 0;
    auto recieve_Image_START = std::chrono::high_resolution_clock::now();

    while (totalBytesReceived < imageSize_Server)
    {
        bytesReceived = recv(clientSocket, threshold_image_buffer, sizeof(threshold_image_buffer), 0);
        if (bytesReceived == -1)
        {
            std::cerr << "Failed to receive picture data" << std::endl;
            break;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Connection closed by peer." << std::endl;
            break;
        }
        incoming_Threshold_Image.write(threshold_image_buffer, bytesReceived);
        totalBytesReceived += bytesReceived;
    }
    auto recieve_Image_END = std::chrono::high_resolution_clock::now();
    auto recieve_Image_duration = std::chrono::duration_cast<std::chrono::milliseconds>(recieve_Image_END - recieve_Image_START);
    printf("\nReciving thresholded image from WINDOWS machine : %" PRId64 " MILLISECONDS or %lf 10^-3 SECONDS\r\nNumber of bytes processed : [ %d ] out of [ %d ] \r\n", recieve_Image_duration.count(), static_cast<double>(recieve_Image_duration.count()) / MILLISECOND_CONVERSION, totalBytesReceived, imageSize_Server);

    // printf("Number of bytes processed : [ %d ] out of [ %d ] \r\n", totalBytesReceived, imageSize_Server);

    // -------------------------- [ Close both saving file stream and socket connection to windows machine ] -------------------------- //
    incoming_Threshold_Image.close();
    close(clientSocket);

    // -------------------------- [ Save threshold image as cv::Mat ] -------------------------- //

    // cv::Mat threshold_image_MAT = cv::imread("/home/pi/Desktop/Onboard_VS_Streaming_RPI/Send_Picture_RPI/build/threshold_image.jpg");
    cv::Mat threshold_image_MAT = cv::imread(TRESHOLD_FILE_SAVE_PATH);
    if (threshold_image_MAT.empty())
    {
        std::cerr << "Error: Unable to load the image." << std::endl;
        return -1;
    }
    cv::imshow("Threshold image", threshold_image_MAT);
    printf("Press ANY key to END the program....\r\n");
    cv::waitKey(0);

    return 0;
}
