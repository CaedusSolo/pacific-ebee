#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

namespace ANSI {  
    static constexpr const char* BLUE = "\033[34m";  
    static constexpr const char* RESET = "\033[0m";  
    static constexpr const char* ORANGE = "\033[33m";  
    static constexpr const char* RED = "\033[31m";  
    static constexpr const char* SLOW_BLINK = "\033[5m";  
    static constexpr const char* RAPID_BLINK = "\033[6m";  
    static constexpr const char* STOP_BLINK = "\033[25m";  
    static constexpr const char* BOLD = "\033[1m";  
    static constexpr const char* NORMAL_WEIGHT = "\033[22m";  
    static constexpr const char* RESET_ALL = "\033[0m";  
} 
class Renderer{
    public:
        // ansi escape codes for colours, blinking and bold, as well as resetting shtuff
        void log(const string& message);
        void render(char* field);
};



#endif