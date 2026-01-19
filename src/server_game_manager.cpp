#include "server_game_manager.h"
#include <iostream>

ServerGameManager::ServerGameManager(){
    isGameOver = false;
    currentPlayerIndex = 0;
    numPlayers = players.size();
}

void ServerGameManager::gameLoop(){
    while(!isGameOver){
        // 1. Get current player
        Player& current = players[currentPlayerIndex];

        // 2. Render the grid for the current play  er
        renderer.drawGame(current,shotHistory);

        // 3. Get Input & Process Attack
        // assume that each player obj will store their own occupied coords
        Vector2D target = current.getInput();
        vector<Player*> hitList = checkPlayerHit(target);

        if (!hitList.empty()) {
            std::cout << "Hit!" << std::endl;
            shotHistory.push_back({target,true});
            // Check win condition...
        } else {
            shotHistory.push_back({target,false});
            // move to next player and reset to first player if overflow
            currentPlayerIndex = (currentPlayerIndex+1) % numPlayers;
        }
    }
    renderer.printFinalResults();
}


vector<Player*> ServerGameManager::checkPlayerHit(Vector2D target){
    vector<Player*> hitPlayers;
    for(int i = 0;i<numPlayers;i++){
        // if a player is hit, return the player hit
        // this assumes that the hit player's ship/coordinate hitting will be marked
        // and also the hit coord will be updated in every player's grid
        if(i!= currentPlayerIndex && players[i].checkHit(target)){
            hitPlayers.push_back(&players[i]);
        }
    }
    return hitPlayers;
}