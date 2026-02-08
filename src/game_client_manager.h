#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "constants.h"
#include "messages.h"
#include "vector2d.h"
#include "connection.h"
#include "renderer.h"
#include "game_states.h"
#include "battlefield.h"
#include "player.h"



typedef struct GameClientData {
    int fd;
    int battlefield_size;
    int player_count;
    int our_index;
    PlayerName player_names[MAX_PLAYER_NUM];
    char game_board[MAX_BATTLEFIELD_SIZE * MAX_BATTLEFIELD_SIZE];
} GameClientData;

GameClientData game_client_manager_create(int fd);
void handle_game_start(GameClientData* game_data);
void handle_turn(GameClientData* game_data);
void handle_game_update(GameClientData* game_data);
int coord_to_index(Vector2D vector2d, int width);
// void listenForNewBattlefield();
//
// Vector2D askShotCoords();
// HitResult sendShot(Vector2D coords);
// HitResult listenForOpponentShot();
static void processHitResult(const HitResult* result, bool isOpponent);
//
// GameStatus listenForGameStatus();
//
static void endGame(bool isWinner);
static int listenForFinalScore(int fd);

#endif // !GAME_MANAGER_H
