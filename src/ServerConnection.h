#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "Connection.h"

class ServerConnection : public Connection {
private:
    int portNumber;

public:
    ServerConnection(int portNum);

    void startConnection();

    Connection* acceptPlayer();
};

#endif
