#include "renderer.h"

void Renderer::log(const string& message){
    // prints normally if passed a normal message,
    // prints colored stuff if passed consoleBuffer
    cout << message << endl;
};
void Renderer::render(char* field){
    cout << field;
}

