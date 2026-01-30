#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "Connection.h"
class ServerConnection {
private:
    int portNumber;
    int serverFD;

public:
    ServerConnection(int portNum);

    ~ServerConnection();
    // Calling the destructor explicitly seems like not doing the correct thing
    // Add another method to close the listening socket
    void stopListening();

    void startConnection();

    int acceptPlayer();
};

#endif
