#include <iostream>
using namespace std;

class ServerConnection {
    void startConnection() {
        cout << "Connection started";
    }
}

class Connection {
    void sendMessage() {
        cout << "Message is being sent by Connection class";
    }

    void listenForMessage() {
        cout << "Connection class is listening for messages";
    }
}

int main (int argc, char *argv[]) {
    
    return 0;
}
