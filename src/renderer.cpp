#include "renderer.h"
#include <iomanip>

void Renderer::log(const string& message){
    // prints normally if passed a normal message,
    // prints colored stuff if passed consoleBuffer
    cout << message << endl;
};
void Renderer::render(char* field){
    cout << field;
}

void Renderer::grid(int playerCount, char battlefield[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE]){
    int size = 10 + (playerCount -2) * 5;  // each player increase, grid increases by 5
    if (size > BATTLEFIELD_SIZE) size = BATTLEFIELD_SIZE;

    cout << "   "; 

    //column
    for (int i = 0; i < size; i++) {
        if (i < 26) { 
            cout << " " << (char)('A' + i) << " ";
        } else {
            cout << " " << (char)('a' + (i - 26)) << " "; // if grid exceeds max UPPERCASE alphabets, continue with LOWERCASE 
        }
        
    }
    cout << endl;

    //row
    for (int r = 0; r < size; r++) {
        cout << setw(2) << (r + 1) << " ";

        for (int c = 0; c < size; c++) {
            char cell = battlefield[r][c];

            if (cell == 'X' || cell = 'O') {
                cout << ANSI::BG_GREY; // attacked area turns grey
            } else {
                cout << ANSI::BG_BLUE; // colour the bg blue 
            }

            if (cell == 'X') {
                cout << ANSI::RED << ANSI::BOLD << " X ";
            } 
            else if (cell == 'O') {
                cout << ANSI::BLUE << " O ";
            } 
            else {
                cout << " . ";
            }

            cout << ANSI::RESET_ALL;
            
        }
        cout << endl;
    }
}

