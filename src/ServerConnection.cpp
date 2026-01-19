#include "ServerConnection.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

ServerConnection::ServerConnection(int portNum) : portNumber(portNum) {
    serverFD = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFD < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

ServerConnection::~ServerConnection() {
    if (serverFD >= 0) {
        close(serverFD);
        cout << "Server listener (FD " << serverFD << ") closed." << endl;
    }
}

void ServerConnection::startConnection() {
    struct sockaddr_in address;
    int opt = 1;

    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portNumber);

    if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverFD, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << portNumber << "..." << endl;
}

Connection* ServerConnection::acceptPlayer() {
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    cout << "Waiting for connection..." << endl;

    int newFD = accept(serverFD, (struct sockaddr*)&clientAddr, (socklen_t*)&addrLen);

    if (newFD < 0) {
        perror("Accept failed. Aborting now.");
        return nullptr;
    }

    return new Connection(newFD);
}
