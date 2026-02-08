#include "server_game_manager.h"
#include "battlefield.h"
#include "connection.h"
#include "constants.h"
#include "game_states.h"
#include "messages.h"
#include "player.h"
#include "shared_memory.h"
#include "logger.h"
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

// replicate renderer but for log
void log_battlefield(SharedMemory* shm, Battlefield* bf) {
    log_event(shm, "--- BATTLEFIELD STATE ---");

    // Header Row (A B C ...)
    char header[512];
    char* ptr = header;
    ptr += sprintf(ptr, "   ");
    for (int x = 0; x < battlefield_get_width(bf); x++) {
        if (x < 26) ptr += sprintf(ptr, " %c ", 'A' + x);
        else ptr += sprintf(ptr, " %c ", 'a' + (x - 26));
    }
    log_event(shm, header);

    // Grid Rows
    for (int y = 0; y < battlefield_get_height(bf); y++) {
        char row_str[512];
        ptr = row_str;
        ptr += sprintf(ptr, "%2d ", y + 1); // Row Number (1-based)

        for (int x = 0; x < battlefield_get_width(bf); x++) {
            Cell* cell = battlefield_get_cell(bf, x, y);
            char sym = '.';

            // Check ship presence
            bool has_ship = false;
            for (int p = 0; p < shm->player_num; p++) {
                if (cell->has_ship[p]) {
                    has_ship = true;
                    break;
                }
            }

            if (cell->is_shot) {
                if (has_ship) sym = 'X'; // Hit
                else sym = 'O'; // Miss
            } else {
                if (has_ship) sym = 'S'; // Ship
                else sym = '.'; // Empty
            }

            ptr += sprintf(ptr, " %c ", sym);
        }
        log_event(shm, row_str);
    }
    log_event(shm, "-------------------------");
}

void* game_update_thread(void *arg) {
    ThreadArgs* thr_args = (ThreadArgs*)arg;
    SharedMemory* shm = thr_args->shm;
    int our_index = thr_args->our_index;
    int fd = thr_args->fd;

    char msg[272];
    // Might be a synchronization problem later
    while (1) {
        sem_wait(&shm->game_update);
        if (shm->current_player_index == our_index)
            continue;
        send_message(fd, GAME_UPDATE, strlen(GAME_UPDATE));
        int msg_len = hitresult_serialize(&shm->hit_result, msg);
        send_message(fd, msg, msg_len);
    }

    return NULL;
}

void handle_player(Player player, int player_index, SharedMemory* shm) {
    PlayerName name;
    listen_for_message(player.fd, name);
    strcpy(player.name, name);
    strcpy(shm->players[player_index].name, name);

    // Log Connection
    log_event(shm, "CONNECTION: Player %d connected as '%s'", player_index, name);

    memset(player.ship_hits, 0, ALL_SHIPS_COUNT);

    printf("[Child %d] Connected. Waiting for other players...\n", player_index);

    pthread_barrier_wait(&shm->game_start_barrier);

    printf("[Child %d] Game Started! Sending signal to client.\n", player_index);
    send_message(player.fd, GAME_START, strlen(GAME_START));

    // Send ship arrays
    // Get the main process to generate the array
    sem_wait(&shm->get_ships_array[player_index]);
    send_message(player.fd, shm->ships_array, BATTLEFIELD_SIZE * BATTLEFIELD_SIZE);
    sem_post(&shm->done_ships_array);

    // Send all players name
    char names_buffer[256 * PLAYER_NUM];
    for (int i = 0; i < PLAYER_NUM; i++) {
        memcpy(names_buffer + (i*256), shm->players[i].name, 256);
    }
    send_message(player.fd, names_buffer, 256*PLAYER_NUM);

    // Send the player index
    char player_index_buffer[4];
    int_serialize(player_index_buffer, player_index);
    send_message(player.fd, player_index_buffer, 4);

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
            send_message(player.fd, GAME_OVER, strlen(GAME_OVER));
            break;
        }

        // --- PLAY TURN ---
        printf("[Child %d] It is my turn.\n", player_index);

        // -- Client/Child process interactions

        // Notify client it is their turn
        sem_wait(&shm->notify_turn_sem);
        send_message(player.fd, YOUR_TURN, strlen(YOUR_TURN));

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

        // Log the shot attempt
        log_event(shm, "ACTION: %s shot at (%d, %d)",
                  shm->players[player_index].name,
                  shm->shoot_position.x,
                  shm->shoot_position.y);

        printf("[Child %d] Player shot!\n", player_index);
        sem_post(&shm->client_shot);

        sem_wait(&shm->complete_game_update);

        // --- END TURN ---
        // Signal the Scheduler that we are done.
        sem_post(&shm->complete_turn_sem);
    }

    close(player.fd);
    pthread_join(updater_thread, NULL);
    printf("[Child %d] Exiting.\n", player_index);
}

void game_loop(SharedMemory *shm, Battlefield* battlefield) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        battlefield_to_char_array(battlefield, i, shm->ships_array);
        sem_post(&shm->get_ships_array[i]);
        sem_wait(&shm->done_ships_array);
    }

    log_event(shm, "GAME START: All players ready.");
    log_battlefield(shm, battlefield);

    int i = 0;
    while (i < 3) { // Keeping original logic
        // Wait for turn change to finish
        sem_wait(&shm->change_turn_sem);

        sem_post(&shm->notify_turn_sem);
        sem_wait(&shm->turn_notified_sem);

        sem_wait(&shm->client_shot);
        // Notify all child processes
        for (int k = 0; k < PLAYER_NUM-1; k++) {
            sem_post(&shm->game_update);
        }

        // Update the battlefield
        printf("Player %d (%s) shot at (%d,%d)\n",
               shm->current_player_index,
               shm->players[shm->current_player_index].name,
               shm->shoot_position.x,
               shm->shoot_position.y
        );

        Vector2D pos = shm->shoot_position;
        Cell* cell = battlefield_get_cell(battlefield, pos.x, pos.y);

        if (cell) cell->is_shot = true;

        shm->hit_result.position = pos;
        shm->hit_result.type = MISS;
        shm->hit_result.attacker_index = shm->current_player_index;

        char* attacker_name = shm->players[shm->current_player_index].name;
        bool hit_detected = false;

        // Check who got shot
        for (int p = 0; p < PLAYER_NUM; p++) {
            if (!cell || !cell->has_ship[p])
                continue;

            enum Ship ship_type = cell->ship_types[p];
            int* ship_hits = &shm->players[p].ship_hits[ship_type];
            *ship_hits += 1;
            shm->players[shm->current_player_index].score += HIT_SCORE;
            shm->hit_result.type = HIT;
            shm->hit_result.victim_index = p;

            hit_detected = true;
            char* victim_name = shm->players[p].name;

            // The ship got hit in all places
            if (*ship_hits == get_ship_size(ship_type)) {
                shm->players[p].is_sunk[ship_type] = true;
                shm->players[shm->current_player_index].score += SUNK_SCORE;
                shm->hit_result.type = SINK;

                log_event(shm, "RESULT: SINK! %s sunk %s's ship!", attacker_name, victim_name);
            } else {
                log_event(shm, "RESULT: HIT! %s hit %s's ship!", attacker_name, victim_name);
            }
        }

        if (!hit_detected) {
            log_event(shm, "RESULT: MISS! %s missed at (%d, %d).", attacker_name, pos.x, pos.y);
        }

        // Log the board state after the move
        log_battlefield(shm, battlefield);

        i++;
        if (i == 3) {
            printf("Game finished\n");
            shm->is_game_over = true;
            log_event(shm, "GAME OVER: Turn limit reached.");
        }

        sem_post(&shm->complete_game_update);
    }
}
