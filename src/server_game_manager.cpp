#include "server_game_manager.h"

#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

ServerGameManager::ServerGameManager(int numPlayers, ServerConnection *serverConn) {
    this->serverConnection = serverConn;
    this->numPlayers = numPlayers;

    isGameOver = false;
    players.reserve(numPlayers);
    currentPlayerIndex = 0;
}

void ServerGameManager::startGame(){
    // Start server connection
    serverConnection->startConnection();

    // Accept player connections and get player name
    for(int i = 0;i<numPlayers;i++){
        int connFD = serverConnection->acceptPlayer();

        Player newPlayer;
        newPlayer.connection = new Connection(connFD);

        // Client should send player name upon connection
        string playerName = newPlayer.connection->listenForMessage();
        newPlayer.name = playerName;
        players.push_back(newPlayer);
    }

    serverConnection->stopListening();

    // Close the listening socker before forking
    // Avoid child processes listening on the same socket
    for (int i = 0; i < numPlayers; i++) {
        currentPlayerIndex = i;
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            handlePlayer();
            return;
        }
        else {
            cout << "Player connected: " << players[i].name << endl;
        }
    }

    // Start game loop
    gameLoop();
}

void ServerGameManager::handlePlayer(){
    cout << "User " << currentPlayerIndex << " connected" << endl;
}

void ServerGameManager::gameLoop(){
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
