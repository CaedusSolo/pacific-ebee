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

#define BF_OUR_SHIP 'S'
#define BF_OUR_SHIP_ATTACKED 'a'
#define BF_OTHER_SUNK_SHIP 's'
#define BF_OUR_ATTACK 'X'
#define BF_OTHER_ATTACK 'x'
#define BF_EMPTY ' '

// player count, battlefield size, player (own) grid?
void grid(int size, char battlefield[MAX_BATTLEFIELD_SIZE * MAX_BATTLEFIELD_SIZE]);


#endif
