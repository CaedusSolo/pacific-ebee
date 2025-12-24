#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
class Renderer{
    private:
        stringstream consoleBuffer;
    public:
        void log(const string& message){
            // prints normally if passed a normal message,
            // prints colored stuff if passed consoleBuffer
            cout << message << endl;
        };
        void error(const string& message){
            string errorMessage = "Error: " + message;
            log(errorMessage);
        };
        void setBufferColor(const char* color){
            cout << "setting buffer color to: " << color;
            consoleBuffer << color;
        };
        void resetBufferColor(){
            cout << "reset buffer color to default";
            consoleBuffer << "\033[0m"; // Reset color to default
        };
        void bufferedLog(const string& message){
            // add msg to console buffer with the current color
            cout << "Adding message to consoleBuffer." <<endl;
            consoleBuffer << message;
        };
        void flushBufferedLog(){
            cout << "Flushing buffered log" << endl;
            // print all the colored stuff to console, and clear it after
            if (consoleBuffer.str().empty()) return; // No buffered log to flush
            log(consoleBuffer.str());
            consoleBuffer.str(""); // Clear the buffer after flushing
        };
};

