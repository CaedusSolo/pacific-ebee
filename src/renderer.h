#ifndef renderer.h
#define renderer.h

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
class Renderer{
    private:
        stringstream consoleBuffer;
    public:
        // ansi escape codes for colours, blinking and bold, as well as resetting shtuff
        const char* BLUE_COLOR = "\033[34m";
        const char* RESET_COLOR = "\033[0m";
        const char* ORANGE_COLOR = "\033[33m";
        const char* RED_COLOR = "\033[31m";
        const char* SLOW_BLINK = "\033[5m";
        const char* RAPID_BLINK = "\033[6m";
        const char* STOP_BLINK = "\033[25m";
        const char* BOLD = "\033[1m";
        const char* NORMAL_WEIGHT = "\033[22m";
        const char* RESET_ATTRIBUTES = "\033[0m";

        void log(const string& message);
        void render(char* field);
};



#endif