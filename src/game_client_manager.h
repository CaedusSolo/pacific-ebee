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



typedef struct GameClientManager {
    Renderer* renderer;

    Battlefield* battlefield;
    int fd;


} GameClientManager;

GameClientManager game_client_manager_create(int fd, Renderer* renderer);
void game_loop(GameClientManager* game_client_manager);
static void waitForGameReady();
static void sendReadySignal();

static char* askPlayerName();
static void sendPlayerName(const PlayerName name);
//
// void listenForNewBattlefield();
// bool listenForFirstTurnDecision();
//
// Vector2D askShotCoords();
// HitResult sendShot(Vector2D coords);
// HitResult listenForOpponentShot();
static void processHitResult(const HitResult& result, bool isOpponent);
//
// GameStatus listenForGameStatus();
//
static void endGame(bool isWinner);
static int listenForFinalScore(int fd);

#endif // !GAME_MANAGER_H
