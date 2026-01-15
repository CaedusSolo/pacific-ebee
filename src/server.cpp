#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Connection.h"

using namespace std;

class ServerConnection : public Connection {
private:
    int port;

public:
    ServerConnection(int portNum) 
        : Connection(socket(AF_INET, SOCK_STREAM, 0)), port(portNum) {
        if (getFD() < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
    }

    void startConnection() {
        struct sockaddr_in address;
        int opt = 1;

        if (setsockopt(getFD(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port); // Host to Network Short

        if (bind(getFD(), (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(getFD(), 2) < 0) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }

        cout << "Server listening on port " << port << "..." << endl;
    }
    // create individual player connections
    Connection* acceptPlayer() {
        struct sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        cout << "Waiting for connection..." << endl;

        int newFD = accept(getFD(), (struct sockaddr*)&clientAddr, (socklen_t*)&addrLen);

        if (newFD < 0) {
            perror("Accept failed");
            return nullptr;
        }

        // return a new wrapper for client's socket
        return new Connection(newFD); 
    }
};
