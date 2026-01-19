#include "server_game_manager.h"

ServerGameManager::ServerGameManager(){
    isGameOver = false;
    currentPlayerIndex = 0;
}

void ServerGameManager::gameLoop(){
    while(!isGameOver){
        // 1. Get current player
            Player& current = players[currentPlayerIndex];
            Player& opponent = players[(currentPlayerIndex + 1) % 2];

            // 2. Render
            renderer.drawGame(current, opponent);

            // 3. Get Input & Process Attack
            // The loop doesn't need "occupied coords" directly;
            // it asks the Opponent object if it got hit.
            Coordinate target = current.getInput();
            bool isHit = opponent.receiveAttack(target);

            if (isHit) {
                std::cout << "Hit!" << std::endl;
                // Check win condition...
            } else {
                // Switch turn only on miss? (Depends on your rules)
                currentPlayerIndex = (currentPlayerIndex + 1) % 2;
            }
        }
    }
    }
}