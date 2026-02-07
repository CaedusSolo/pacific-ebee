#include "renderer.h"
#include "constants.h"
#include <stdio.h>

void displayBanner(void) {
    printf(ANSI_BLUE " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n");
    printf("|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|\n" ANSI_RESET);
    printf("                                                                                           \n");
    printf("       ____    ____     __   ____  _____  ____     __         ___  ____     ___    ___       \n");
    printf("      |    \\ /    |   /  ] |    ||     ||    |   /  ]       /  _]|    \\  /  _]  /  _]      \n");
    printf("      |  o  )|  o  |  /  /   |  | |   __| |  |   /  /       /  [_ |  o  ) /  [_  /  [_       \n");
    printf("      |   _/ |     | /  /    |  | |  |_   |  |  /  /       |    _]|     ||    _]|    _]      \n");
    printf("      |  |   |  _  |/   \\_  |  | |   _]  |  | /   \\_     |   [_ |  O  ||   [_ |   [_       \n");
    printf("      |  |   |  |  |\\     | |  | |  |    |  | \\     |    |     ||     ||     ||     |      \n");
    printf("      |__|   |__|__| \\____||____||__|   |____| \\____|    |_____||_____||_____||_____|      \n");
    printf("                                                                                           \n");
    printf("                                                                                           \n");
    printf("                                _______/_____\\_______\\_____                                \n");
    printf("                                \\              < < <       |                               \n");
    printf("                              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                           \n");
    printf(ANSI_BLUE " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n");
    printf("|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|\n" ANSI_RESET);
}


void grid(int playerCount, char battlefield[BATTLEFIELD_SIZE * BATTLEFIELD_SIZE], bool isOurBoard){
    int size = 10 + (playerCount - 2) * 5;  // each player increase, grid increases by 5
    if (size > BATTLEFIELD_SIZE) size = BATTLEFIELD_SIZE;

    printf("   ");

    // column
    for (int i = 0; i < size; i++) {
        if (i < 26) {
            printf(" %c ", 'A' + i);
        } else {
            printf(" %c ", 'a' + (i - 26)); // if grid exceeds max UPPERCASE alphabets, continue with LOWERCASE
        }
    }
    printf("\n");

    // row
    for (int r = 0; r < size; r++) {
        printf("%2d ", r + 1);

        for (int c = 0; c < size; c++) {
            int index = c + (r * BATTLEFIELD_SIZE);

            char cell = battlefield[index];

            if ((cell == 'S' && isOurBoard) || cell == 'X' || cell == 'O') {
                printf("%s", ANSI_BG_GREY); // attacked area turns grey
            } else {
                printf("%s", ANSI_BG_BLUE); // colour the bg blue
            }

            if (cell == 'X') {
                printf("%s%s X ", ANSI_RED, ANSI_BOLD);
            }
            else if (cell == 'O') {
                printf(" O ");
            }
            else if (cell == 'S' && isOurBoard) {
                printf(" S "); // placement of the ships
            }
            else {
                printf(" . ");
            }

            printf("%s", ANSI_RESET_ALL);
        }
        printf("\n");
    }
}

