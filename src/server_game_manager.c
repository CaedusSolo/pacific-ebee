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

    char msg[500];
    // Might be a synchronization problem later
    while (1) {
        sem_wait(&shm->game_update);
        if (shm->current_player_index != our_index)
            send_message(fd, GAME_UPDATE, strlen(GAME_UPDATE));
        int msg_len = hitresult_serialize(&shm->hit_result, msg);
        send_message(fd, msg, msg_len);
        sem_post(&shm->complete_game_update);
        if (shm->is_game_over) break;
    }

    return NULL;
}

void send_init_data(Player player, int player_index, SharedMemory* shm) {
    // Send ship arrays
    // Get the main process to generate the array
    sem_wait(&shm->get_ships_array[player_index]);
    size_t ships_array_len = shm->battlefield_size * shm->battlefield_size;
    send_message(player.fd, shm->ships_array, ships_array_len);
    sem_post(&shm->done_ships_array);

    // Send player count
    char player_num_buffer[4];
    int_serialize(player_num_buffer, shm->player_num);
    send_message(player.fd, player_num_buffer, 4);

    // Send player index
    char their_index_buffer[4];
    int_serialize(their_index_buffer, player_index);
    send_message(player.fd, their_index_buffer, 4);

    // Send all players name
    char names_buffer[256 * shm->player_num];
    for (int i = 0; i < shm->player_num; i++) {
        memcpy(names_buffer + (i*256), shm->players[i].name, 256);
    }
    send_message(player.fd, names_buffer, 256*shm->player_num);

    // Send the battlefield size
    char battlefield_size_buffer[4];
    int_serialize(battlefield_size_buffer, shm->battlefield_size);
    send_message(player.fd, battlefield_size_buffer, 4);
}

void handle_player(Player player, int player_index, SharedMemory* shm) {
    PlayerName name;
    listen_for_message(player.fd, name);
    strcpy(player.name, name);
    strcpy(shm->players[player_index].name, name);

    // Log Connection
    log_event(shm, "CONNECTION: Player %d connected as '%s'", player_index, name);

    memset(player.ship_hits, 0, ALL_SHIPS_COUNT * sizeof(int));

    printf("[Child %d] Connected. Waiting for other players...\n", player_index);

    pthread_barrier_wait(&shm->game_start_barrier);

    printf("[Child %d] Game Started! Sending signal to client.\n", player_index);
    send_message(player.fd, GAME_START, strlen(GAME_START));

    send_init_data(player, player_index, shm);

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
            char score_buffer[4];
            int_serialize(score_buffer, shm->players[player_index].score);
            send_message(player.fd, score_buffer, 4);
            break;
        }

        // --- PLAY TURN ---
        printf("[Child %d] It is my turn.\n", player_index);

        // -- Client/Child process interactions

        // Notify client it is their turn
        sem_wait(&shm->notify_turn_sem);
        send_message(player.fd, YOUR_TURN, strlen(YOUR_TURN));

        char resp[100];
        // do {
        listen_for_message(player.fd, resp);
        // } while(strcmp(resp, READY_FOR_TURN) != 0);

        sem_post(&shm->turn_notified_sem);

        // Wait for client input shooting position (Blocking read)
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

        for (int i = 0; i < shm->player_num; i++) {
            sem_wait(&shm->complete_game_update);
        }

        // --- END TURN ---
        // Signal the Scheduler that we are done.
        printf("[Child %d] Done turn\n", player_index);
        sem_post(&shm->complete_turn_sem);
    }

    close(player.fd);
    pthread_join(updater_thread, NULL);
    printf("[Child %d] Exiting.\n", player_index);
}

void update_score_infos(SharedMemory *shm) {
    for (int i = 0; i < shm->player_num; i++) {
        ScoreInfo* score_info = &shm->score_infos[i];
        for (int j = 0; j < shm->player_num; j++) {
            Player* player = &shm->players[j];
            if (!strcmp(score_info->name, player->name)) {
                if (player->score > score_info->highscore)
                    score_info->highscore = player->score;
                score_info->previous_score = player->score;
            }
        }
    }
}

void game_loop(SharedMemory *shm, Battlefield* battlefield) {
    for (int i = 0; i < shm->player_num; i++) {
        battlefield_to_char_array(battlefield, i, shm->ships_array);
        sem_post(&shm->get_ships_array[i]);
        sem_wait(&shm->done_ships_array);
    }

    log_event(shm, "GAME START: All players ready.");
    log_battlefield(shm, battlefield);

    while (1) {
        // Wait for turn change to finish
        sem_wait(&shm->change_turn_sem);

        if (shm->is_game_over) {
            break;
        }

        sem_post(&shm->notify_turn_sem);
        sem_wait(&shm->turn_notified_sem);

        sem_wait(&shm->client_shot);

        Player* attacker = &shm->players[shm->current_player_index];

        // Update the battlefield
        printf("Player %d (%s) shot at (%d,%d)\n",
               shm->current_player_index,
               attacker->name,
               shm->shoot_position.x,
               shm->shoot_position.y
        );

        Vector2D pos = shm->shoot_position;
        Cell* cell = battlefield_get_cell(battlefield, pos.x, pos.y);

        if (cell) {
            cell->is_shot = true;
            cell->attacker_index = shm->current_player_index;
        }

        shm->hit_result.position = pos;
        shm->hit_result.type = MISS;
        shm->hit_result.attacker_index = shm->current_player_index;

        bool hit_detected = false;

        // Check who got shot
        for (int i = 0; i < shm->player_num; i++) {
            if (!cell || !cell->has_ship[i])
                continue;

            enum Ship ship_type = cell->ship_types[i];
            Player* victim = &shm->players[i];

            // Update Stats
            victim->ship_hits[ship_type] += 1;
            victim->total_hits += 1;
            attacker->score += HIT_SCORE;

            // Set Result
            shm->hit_result.type = HIT;
            shm->hit_result.victim_index = i;
            hit_detected = true;

            // Game ends if one player have nothing left
            if (victim->total_hits == ALL_SHIPS_COORDS_COUNT) {
                shm->is_game_over = true;
                update_score_infos(shm);
                log_event(shm, "GAME OVER: Player %s has lost all ships.", victim->name);
            }
        }

        if (!hit_detected) {
            log_event(shm, "RESULT: MISS! %s missed at (%d, %d).", attacker->name, pos.x, pos.y);
        }

        // Log the board state after the move
        log_battlefield(shm, battlefield);

        // Notify all child processes
        for (int i = 0; i < shm->player_num; i++) {
            sem_post(&shm->game_update);
        }
    }
}
