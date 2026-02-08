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


void grid(int size, char battlefield[MAX_BATTLEFIELD_SIZE * MAX_BATTLEFIELD_SIZE]){
    if (size > MAX_BATTLEFIELD_SIZE) size = MAX_BATTLEFIELD_SIZE;

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
            int index = c + (r * MAX_BATTLEFIELD_SIZE);

            char cell = battlefield[index];

            switch (cell) {
                case BF_OTHER_ATTACK:
                    printf("%s%s X ", ANSI_RED, ANSI_BOLD);
                	break;
                case BF_OUR_ATTACK:
                    printf("%s X ", ANSI_GREEN);
                	break;
                case BF_OUR_SHIP:
                    printf("%s%s S ", ANSI_BG_BLUE, ANSI_BLUE);
                	break;
                case BF_OUR_SHIP_ATTACKED:
                    printf("%s%s%s X ", ANSI_BG_BLUE, ANSI_RED, ANSI_BOLD);
                case BF_OTHER_SUNK_SHIP:
                    printf("%s%s O ", ANSI_BG_GREY, ANSI_BLUE);
                default:
                    printf(" . ");
            }

            printf("%s", ANSI_RESET_ALL);
        }
        printf("\n");
    }
}

