#include "server_game_manager.h"
#include "connection.h"
#include "constants.h"
#include "messages.h"
#include "shared_memory.h"
#include "vector2d.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef struct ThreadArgs {
    SharedMemory* shm;
    int our_index;
    int fd;
} ThreadArgs;

void* game_update_thread(void *arg) {
    ThreadArgs* thr_args = (ThreadArgs*)arg;
    SharedMemory* shm = thr_args->shm;
    int our_index = thr_args->our_index;
    int fd = thr_args->fd;

    char msg[8];
    // Might be a synchronization problem later
    while (shm->current_player_index != our_index) {
        sem_wait(&shm->game_update);
        send_message(fd, "UPDATE", 7);
        vector2d_serialize(&shm->shoot_position, msg);
        send_message(fd, msg, 8);
    }

    return NULL;
}

void handle_player(Player player, int player_index, SharedMemory* shm) {
    printf("[Child %d] Connected. Waiting for other players...\n", player_index);

    pthread_barrier_wait(&shm->game_start_barrier);

    printf("[Child %d] Game Started! Sending signal to client.\n", player_index);
    send_message(player.fd, "GAME_START", 11);

    // Separate thread to send game update states
    ThreadArgs thr_args = {
        .shm = shm,
        .our_index = player_index,
        .fd = player.fd
    };
    pthread_t updater_thread;
    pthread_create(&updater_thread, NULL, game_update_thread, &thr_args);

    // 2. MAIN LOOP
    while (1) {
        // --- WAIT FOR TURN ---
        sem_wait(&shm->turn_sem[player_index]);

        // Check if game ended while we were waiting
        if (shm->is_game_over) {
            send_message(player.fd, "GAME_OVER", 10);
            break;
        }

        // --- PLAY TURN ---
        printf("[Child %d] It is my turn.\n", player_index);

        // -- Client/Child process interactions

        // Notify client it is their turn
        sem_wait(&shm->notify_turn_sem);
        send_message(player.fd, YOUR_TURN, 10);

        char resp[20];
        // do {
        listen_for_message(player.fd, resp);
        // } while(strcmp(resp, READY_FOR_TURN) != 0);

        sem_post(&shm->turn_notified_sem);

        // Wait for client input shooting position (Blocking read)
        size_t len;
        char input[8];
        listen_for_message(player.fd, input);
        shm->shoot_position = vector2d_deserialize(input);
        printf("[Child %d] Player shot!\n", player_index);
        sem_post(&shm->client_shot);

        // if (!input) {
        //     printf("[Child %d] Client disconnected.\n", player_index);
        //     shm->player_connected[player_index] = false;
        //     sem_post(&shm->complete_turn_sem); // Don't hang the scheduler
        //     break;
        // }


        sem_wait(&shm->complete_game_update);

        // // --- CRITICAL SECTION (Update Shared Memory) ---
        // // Lock mutex to prevent race conditions while updating the board
        // pthread_mutex_lock(&shm->game_state_mutex);
        //
        // printf("[Child %d] Received move: %s\n", player_index, input);
        //
        // pthread_mutex_unlock(&shm->game_state_mutex);
        //
        // // Send result back to client
        // send_message(player.fd, "RESULT: PROCESSED");
        // free(input);

        // --- END TURN ---
        // Signal the Scheduler that we are done.
        sem_post(&shm->complete_turn_sem);
    }

    close(player.fd);
    pthread_join(updater_thread, NULL);
    printf("[Child %d] Exiting.\n", player_index);
}

void game_loop(SharedMemory *shm){
    int i = 0;
    while (i < 3) {
        // Wait for turn change to finish
        sem_wait(&shm->change_turn_sem);

        sem_post(&shm->notify_turn_sem);
        sem_wait(&shm->turn_notified_sem);

        sem_wait(&shm->client_shot);
        // Notify all child processes
        for (int i = 0; i < PLAYER_NUM-1; i++) {
            sem_post(&shm->game_update);
        }
        // Update the battlefield
        printf("Player %d (%s) shot at (%d,%d)\n",
               shm->current_player_index,
               shm->player_names[shm->current_player_index],
               shm->shoot_position.x,
               shm->shoot_position.y
        );

        i++;
        if (i == 3) {
            printf("Game finished\n");
            shm->is_game_over = true;
        }

        sem_post(&shm->complete_game_update);
    }
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
