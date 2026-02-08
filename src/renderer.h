#ifndef RENDERER_H
#define RENDERER_H

#include "constants.h"
#include <stdbool.h>


#define ANSI_BLUE "\033[34m"
#define ANSI_ORANGE "\033[33m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_PINK "\033[95m"

#define ANSI_BG_BLUE "\033[44m"
#define ANSI_BG_GREY "\033[100m"

#define ANSI_RESET "\033[0m"
#define ANSI_SLOW_BLINK "\033[5m"
#define ANSI_RAPID_BLINK "\033[6m"
#define ANSI_STOP_BLINK "\033[25m"
#define ANSI_BOLD "\033[1m"
#define ANSI_NORMAL_WEIGHT "\033[22m"
#define ANSI_RESET_ALL "\033[0m"

// player count, battlefield size, player (own) grid?
void grid(int size, char battlefield[BATTLEFIELD_SIZE * BATTLEFIELD_SIZE], bool isOurBoard);


#endif
