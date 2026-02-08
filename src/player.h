#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "game_states.h"

// Make name maximum to 255 charaters + null termination
typedef char PlayerName[256];

typedef struct Player {
    bool connected;
    int pid;
    // Socket file descriptor
    int fd;
    int score;
    int total_hits;

    // Number of hits each of our ships have gotten
    int ship_hits[ALL_SHIPS_COUNT];
    bool is_sunk[ALL_SHIPS_COUNT];
    PlayerName name;
} Player;

#endif // !PLAYER_H
