#include "server_game_manager.h"
#include "connection.h"
#include "shared_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void handle_player(Player player, int player_index, SharedMemory* shm) {
    printf("[Child %d] Connected. Waiting for other players...\n", player_index);

    pthread_barrier_wait(&shm->game_start_barrier);

    printf("[Child %d] Game Started! Sending signal to client.\n", player_index);
    send_message(player.fd, "GAME_START");

    // 2. MAIN LOOP
    while (1) {
        // --- A. WAIT FOR TURN ---
        sem_wait(&shm->turn_sem[player_index]);

        // Check if game ended while we were waiting
        if (shm->is_game_over) {
            send_message(player.fd, "GAME_OVER");
            break;
        }

        // --- B. PLAY TURN ---
        printf("[Child %d] It is my turn.\n", player_index);
        send_message(player.fd, "YOUR_TURN");

        // Wait for client input (Blocking read)
        size_t len;
        char* input = listen_for_message(player.fd, &len);

        if (!input) {
            printf("[Child %d] Client disconnected.\n", player_index);
            shm->player_connected[player_index] = false;
            sem_post(&shm->complete_turn_sem); // Don't hang the scheduler
            break;
        }

        // --- C. CRITICAL SECTION (Update Shared Memory) ---
        // Lock mutex to prevent race conditions while updating the board
        pthread_mutex_lock(&shm->game_state_mutex);

        printf("[Child %d] Received move: %s\n", player_index, input);

        pthread_mutex_unlock(&shm->game_state_mutex);

        // Send result back to client
        send_message(player.fd, "RESULT: PROCESSED");
        free(input);

        // --- D. END TURN ---
        // Signal the Scheduler that we are done.
        sem_post(&shm->complete_turn_sem);
    }

    close(player.fd);
    printf("[Child %d] Exiting.\n", player_index);
}

void game_loop(ServerGameManager* self){
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
