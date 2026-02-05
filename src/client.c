#include "connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>


int connect_to_server(const char* server_ip, int port);


int main (int argc, char *argv[]) {
    // Get the actual first argument (IP address of server)
    // Note that the first argument (argv[0]) is the program name
    const char* serverIP = "127.0.0.1";
    // Default port 8080 if not provided
    const int port = argv[2] ? strtoumax(argv[2], NULL, 10) : 8999;

    int fd = connect_to_server(serverIP, port);
    if (fd < 0) {
        return 1; // Connection failed
    }
    send_message(fd, argv[1]);
    while (true) {};
}

int connect_to_server(const char* server_ip, int port) {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    int status = inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    if (status == 0) {
        perror("Invalid IP address");
        return -1;
    }

    status = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (status < 0) {
        perror("Error connecting to server");
        return -1;
    }

    return sockfd;
}
