#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT_NUMBER 1025
#define WINDOWS_IP ""

//======================================== [ RASPBERRY PI CODE ] ======================================== //

int main()
{
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Socket creation failed");
        return 1;
    }
    printf("Socket created...\r\n");

    // Server information
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_NUMBER);           // Replace with the port number on the Windows machine
    serverAddr.sin_addr.s_addr = inet_addr(WINDOWS_IP); // Replace with the Windows machine's IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("Connection failed");
        close(clientSocket);
        return 1;
    }
    else
    {
        printf("Connection established\r\n");
    }

    // Message to send
    const char *message = "65014";
    send(clientSocket, message, strlen(message), 0);
    printf("Message sent....\r\n");

    // Close the socket
    close(clientSocket);

    return 0;
}
