#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
class Renderer{
    private:
        stringstream consoleBuffer;
    public:
        Renderer();
        ~Renderer();
        void log(const string& message){
            // use this if logging msg has no color
            cout << message << endl;
        };
        void error(const string& message){
            string errorMessage = "Error: " + message;
            outputToConsole(errorMessage);
        };
        void setBufferColor(const char* color){
            consoleBuffer << color;
        };
        void resetBufferColor(){
            consoleBuffer << "\033[0m"; // Reset color to default
        };
        void bufferedLog(const string& message){
            // add msg to console buffer with the current color
            consoleBuffer << message;
        };
        void flushBufferedLog(){
            // print all the colored stuff to console, and clear it after
            if (consoleBuffer.str().empty()||fileBuffer.str().empty()) return; // No buffered log to flush
            outputToConsole(consoleBuffer.str());
            consoleBuffer.str(""); // Clear the buffer after flushing
        };
};

