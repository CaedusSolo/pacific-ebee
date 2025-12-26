#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
using namespace std;

class Connection {
    private:
        int socketFD;
    public:
        Connection(int fd);

        ~Connection();

        void sendMessage(string message);
        string listenForMessage();
        int getFD();
};

#endif
