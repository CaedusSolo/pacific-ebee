#ifndef SERVER_GAME_MANAGER_H
#define SERVER_GAME_MANAGER_H

#include "vector2d.h"
#include "player.h"
#include "constants.h"
#include "shared_memory.h"
#include "battlefield.h"


typedef struct Shot{
    bool is_hit;
    Vector2D coord;
} Shot;

typedef struct ServerGameManager{
    bool is_game_over;
    int num_players;
    int current_player_index;
    int listening_fd;
    Player players[PLAYER_NUM];
} ServerGameManager;

ServerGameManager server_game_manager_create(int num_players, int listening_fd);
void server_game_manager_destroy(ServerGameManager* self);
void start_game(ServerGameManager* self);
void game_loop(SharedMemory *shm, Battlefield* battlefield);
void handle_player(Player player, int player_index, SharedMemory *shm);
void check_player_hit(ServerGameManager* self);

#endif
