#include <iostream>
#include <vector>
#include <string>

using namespace std;

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
