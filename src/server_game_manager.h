#ifndef SERVER_GAME_MANAGER_H
#define SERVER_GAME_MANAGER_H

#include "vector2d.h"
#include "player.h"
#include "ServerConnection.h"

#include <string>
#include <vector>

using namespace std;

struct Shot{
    Vector2D coord;
    bool isHit;
};

class ServerGameManager{
private:
    vector<Shot> shotHistory;
    vector<Player> players;
    int numPlayers;
    bool isGameOver;

    int currentPlayerIndex;

    ServerConnection *serverConnection;

public:
    ServerGameManager(int numPlayers, ServerConnection *serverConn);
    void startGame();
    void gameLoop();
    void handlePlayer();
    Player* checkPlayerHit();
};

#endif
