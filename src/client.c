#include "connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>


int connectToServer(const char* serverIP, int port);


int main (int argc, char *argv[]) {
    // Get the actual first argument (IP address of server)
    // Note that the first argument (argv[0]) is the program name
    const char* serverIP = "127.0.0.1";
    // Default port 8080 if not provided
    const int port = argv[2] ? strtoumax(argv[2], NULL, 10) : 8999;

    int fd = connectToServer(serverIP, port);
    if (fd < 0) {
        return 1; // Connection failed
    }
    send_message(fd, argv[1]);
    while (true) {};
}

int connectToServer(const char* serverIP, int port) {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return NULL;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    int status = inet_aton(serverIP, &serverAddr.sin_addr);
    if (status == 0) {
        perror("Invalid IP address");
        return -1;
    }

    status = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (status < 0) {
        perror("Error connecting to server");
        return -1;
    }

    return sockfd;
}
