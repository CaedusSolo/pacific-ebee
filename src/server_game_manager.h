#ifndef SERVER_GAME_MANAGER_H
#define SERVER_GAME_MANAGER_H

#include "vector2d.h"
#include "renderer.h"

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
    Renderer renderer;
    int currentPlayerIndex;
public:
    ServerGameManager();
    void gameLoop();
    Player* checkPlayerHit();
};

#endif
