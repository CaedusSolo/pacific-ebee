#ifndef SERVER_GAME_MANAGER_H
#define SERVER_GAME_MANAGER_H

#include "vector2d.h"
#include "player.h"
#include "constants.h"
#include "shared_memory.h"
#include "battlefield.h"

void game_loop(SharedMemory *shm, Battlefield* battlefield);
void handle_player(Player player, int player_index, SharedMemory *shm);

#endif
