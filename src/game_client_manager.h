#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "constants.h"
#include "messages.h"
#include "vector2d.h"
#include "connection.h"
#include "game_states.h"


class GameClientManager {
public:
    GameClientManager(Connection& serverConn);
    ~GameClientManager();

    void gameLoop();

private:
    Renderer& renderer;

    // Might have to abstract this into a Battlefield class later
    char battlefield[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];

    Connection& serverConn;

    void waitForGameReady();
    void sendReadySignal();

    string askPlayerName();
    void sendPlayerName(const string& name);

    void listenForNewBattlefield();
    bool listenForFirstTurnDecision();

    Vector2D askShotCoords();
    HitResult sendShot(Vector2D coords);
    HitResult listenForOpponentShot();
    void processHitResult(const HitResult& result, bool isOpponent);

    GameStatus listenForGameStatus();

    void endGame(bool isWinner);
    int listenForFinalScore();
};

#endif // !GAME_MANAGER_H
