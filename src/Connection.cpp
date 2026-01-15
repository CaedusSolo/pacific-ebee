#include "Connection.h"
#include <iostream>
#include <cstring>      // For memset
#include <unistd.h>     // For read, write, close
#include <sys/socket.h> // For socket definitions

using namespace std;

Connection::Connection(int fd) : socketFD(fd) {}

Connection::~Connection() {
    // close connection if it's a valid file descriptor
    if (socketFD >= 0) {
        close(socketFD);
    }
}

void Connection::sendMessage(string message) {
    if (message.empty() || message.back() != '\n') {
        message += '\n';
    }

    ssize_t bytesSent = write(socketFD, message.c_str(), message.length());

    if (bytesSent < 0) {
        perror("Failed to send message");
    }
}

string Connection::listenForMessage() {
    char buffer[1024];
    // clean buffer to avoid garbage data
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytesRead = read(socketFD, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0) {
        // if message is null terminate the string safely
        buffer[bytesRead] = '\0';
        string received(buffer);
        if (!received.empty() && received.back() == '\n') {
            received.pop_back();
        }
        return received;
    } 
    // 0 = other side closed connection
    else if (bytesRead == 0) {
        return "";
    } 
    else {
        perror("Read failed");
        return "";
    }
}

int Connection::getFD() {
    return socketFD;
}
