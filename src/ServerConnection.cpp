#include "ServerConnection.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>  // for perror
#include <cstdlib> // for exit

using namespace std;

ServerConnection::ServerConnection(int portNum) 
    : Connection(socket(AF_INET, SOCK_STREAM, 0)), portNumber(portNum) {
    
    if (getFD() < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void ServerConnection::startConnection() {
    struct sockaddr_in address;
    int opt = 1;

    if (setsockopt(getFD(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portNumber); 

    if (bind(getFD(), (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(getFD(), 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << portNumber << "..." << endl;
}

Connection* ServerConnection::acceptPlayer() {
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    cout << "Waiting for connection..." << endl;

    // getFD() retrieves the server's listening socket descriptor
    int newFD = accept(getFD(), (struct sockaddr*)&clientAddr, (socklen_t*)&addrLen);

    if (newFD < 0) {
        perror("Accept failed");
        return nullptr;
    }

    // returns a new Connection object wrapping the client's socket
    return new Connection(newFD); 
}
