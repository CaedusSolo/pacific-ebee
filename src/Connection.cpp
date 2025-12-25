#include "Connection.h"
#include <string>
#include <iostream>

using namespace std;

Connection::Connection(int fd) : socketFD(fd) {}

Connection::~Connection() {
    cout << "Destructor for Connection executing..." << endl;
}

void Connection::sendMessage(string message) {
    cout << "Sending to FD " << socketFD << ": " << message << endl;
}

string Connection::listenForMessage() {
    cout << "Listening on FD " << socketFD << "..." << endl;
    return "Stub Message"; 
}

int Connection::getFD() {
    return socketFD;
}
