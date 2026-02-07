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
    Battlefield* battlefield;
    int fd;
} GameClientData;

GameClientData game_client_manager_create(int fd);
void handle_turn(GameClientData* game_client_data);
void handle_game_update(GameClientData* game_client_data);
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
