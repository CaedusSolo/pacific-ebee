#include "server_game_manager.h"

#include <stdio.h>
#include <unistd.h>


ServerGameManager server_game_manager_create(int num_players, int listening_fd) {
    ServerGameManager game_manager = {
        .is_game_over = false,
        .num_players = num_players,
        .listening_fd = listening_fd,
        .current_player_index = 0,
        .players = {}
    };

    return game_manager;
}

void handlePlayer(Player player){
    printf("User (%s) connected\n", player.name);
}

void gameLoop(){
    // while(!isGameOver){
    //     // 1. Get current player
    //     Player& current = players[currentPlayerIndex];
    //
    //     // 3. Get Input & Process Attack
    //     // assume that each player obj will store their own occupied coords
    //     Vector2D target = current.getInput();
    //     vector<Player*> hitList = checkPlayerHit(target);
    //
    //     if (!hitList.empty()) {
    //         std::cout << "Hit!" << std::endl;
    //         shotHistory.push_back({target,true});
    //         // Check win condition...
    //     } else {
    //         shotHistory.push_back({target,false});
    //         // move to next player and reset to first player if overflow
    //         currentPlayerIndex = (currentPlayerIndex+1) % numPlayers;
    //     }
    // }
}


// vector<Player*> ServerGameManager::checkPlayerHit(Vector2D target){
//     vector<Player*> hitPlayers;
//     for(int i = 0;i<numPlayers;i++){
//         // if a player is hit, return the player hit
//         // this assumes that the hit player's ship/coordinate hitting will be marked
//         // and also the hit coord will be updated in every player's grid
//         if(i!= currentPlayerIndex && players[i].checkHit(target)){
//             hitPlayers.push_back(&players[i]);
//         }
//     }
//     return hitPlayers;
// }
