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

    void startConnection();

    Connection* acceptPlayer();
};

#endif 
