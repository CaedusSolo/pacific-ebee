#include <iostream>
#include <vector>
#include <string>
#include "Connection.h"

using namespace std; 

class ClientConnection {
private:
    int clientFD; //stub value here?
    int port;
    string serverAddress;

public: 
    ClientConnection(int portNum, string addr) : serverAddress(addr), port(portNum) {}

    Connection* startConnection() {
        cout << "Connecting to -> " << serverAddress << ":" << port << endl;
        return new Connection(clientFD);
    }

};

int main (int argc, char *argv[]) {
    
    return 0;
}
