#include "Connection.h"
#include <iostream>
#include <cstring>      // For memset
#include <unistd.h>     // For read, write, close
#include <sys/socket.h> // For socket definitions
#include <arpa/inet.h>  // For htonl, ntohl
#include <string>

using namespace std;

Connection::Connection(int fd) : socketFD(fd) {}

Connection::~Connection() {
    // close connection if it's a valid file descriptor
    if (socketFD >= 0) {
        close(socketFD);
    }
}

void Connection::sendMessage(string outgoingMessage) {
    
    // 4 bytes
    uint32_t messageLength = outgoingMessage.length();

    // convert to big endian
    uint32_t networkOrderLength = htonl(messageLength);

    // 2. Send the Header (First 4 bytes)
    ssize_t bytesWrittenHeader = write(socketFD, &networkOrderLength, sizeof(networkOrderLength));

    if (bytesWrittenHeader != sizeof(networkOrderLength)) {
        perror("Failed to send length of message");
        return; 
    }

    ssize_t totalBytesWritten = 0;
    while (totalBytesWritten < messageLength) {
        const char* remainingDataPointer = outgoingMessage.c_str() + totalBytesWritten;
        size_t bytesRemaining = messageLength - totalBytesWritten;

        ssize_t bytesWrittenCurrentChunk = write(socketFD, remainingDataPointer, bytesRemaining);
        
        if (bytesWrittenCurrentChunk < 0) {
            perror("Failed to send message body");
            return;
        }
        totalBytesWritten += bytesWrittenCurrentChunk;
    }
}

string Connection::listenForMessage() {
    // 4 bytes
    uint32_t networkOrderLength = 0;

    char* headerBufferPointer = (char*)&networkOrderLength;

    ssize_t totalHeaderBytesRead = 0;
    size_t headerSize = sizeof(networkOrderLength);

    // Loop until received exactly 4 bytes
    while (totalHeaderBytesRead < headerSize) {
        ssize_t bytesReadCurrentChunk = read(socketFD, headerBufferPointer + totalHeaderBytesRead, headerSize - totalHeaderBytesRead);
        
        if (bytesReadCurrentChunk == 0) return ""; // client closed connection
        if (bytesReadCurrentChunk < 0) {
            perror("Read header failed. Aborting now.");
            return "";
        }
        totalHeaderBytesRead += bytesReadCurrentChunk;
    }

    // convert to big endian
    uint32_t messageLength = ntohl(networkOrderLength);


    // read message body
    vector<char> messageBuffer(messageLength + 1);
    ssize_t totalMessageBytesRead = 0;

    while (totalMessageBytesRead < messageLength) {
        char* writeLocation = &messageBuffer[totalMessageBytesRead];
        size_t bytesToRead = messageLength - totalMessageBytesRead;

        ssize_t bytesReadCurrentChunk = read(socketFD, writeLocation, bytesToRead);
        
        if (bytesReadCurrentChunk <= 0) {
            perror("Read body failed or connection closed mid-message");
            return "";
        }
        totalMessageBytesRead += bytesReadCurrentChunk;
    }

    // conver to string
    messageBuffer[messageLength] = '\0';
    return string(messageBuffer.data());
}

int Connection::getFD() {
    return socketFD;
}
