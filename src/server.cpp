#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Connection {
    private:
        int socketFD; 

public:
    Connection(int fd) : socketFD(fd) {}

    ~Connection() {
        cout << "Destructor for Connection executing...";
    }

    void sendMessage(string message) {
        cout << "Sending to FD " << socketFD << ": " << message << endl;
    }

    string listenForMessage() {
        cout << "Listening on FD " << socketFD << "..." << endl;
        return "Stub Message"; 
    }

    int getFD() { return socketFD; }
};

class ServerConnection {
private:
    int port = 8000;
    int serverFD;

public:
    ServerConnection(int portNum) : port(portNum) {}

    void startConnection() {
        cout << "Server started on port " << port << endl;
    }

};

int main (int argc, char *argv[]) {

    return 0;
}
