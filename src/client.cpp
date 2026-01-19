#include <string>
#include "Connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>

using namespace std;

Connection* connectToServer(string serverIP, int port);


int main (int argc, char *argv[]) {
    // Get the actual first argument (IP address of server)
    // Note that the first argument (argv[0]) is the program name
    const char* serverIP = argv[1];
    const int port = argv[2] ? stoi(argv[2]) : 8080; // Default port 8080 if not provided
}

Connection* connectToServer(string serverIP, int port) {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return nullptr;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    int status = inet_aton(serverIP.c_str(), &serverAddr.sin_addr);
    if (status == 0) {
        perror("Invalid IP address");
        return nullptr;
    }

    status = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (status < 0) {
        perror("Error connecting to server");
        return nullptr;
    }

    return new Connection(sockfd);
}
