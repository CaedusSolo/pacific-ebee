#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define _XOPEN_SOURCE 700

#include "messages.h"
#include "constants.h"
#include "player.h"
#include "vector2d.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct {
   char message[MAX_LOG_LENGTH] ;
} LogMessage;

typedef struct {
    int player_num;
    bool game_running;
    bool is_game_over;
    int current_player_index;
    int battlefield_size;

    // Messaging between main process and child processes
    Vector2D shoot_position;

    HitResult hit_result;

    pthread_mutex_t logger_mutex;
    sem_t log_count_sem;
    pthread_mutex_t game_state_mutex;

    sem_t notify_turn_sem;
    sem_t turn_notified_sem;

    sem_t change_turn_sem;
    sem_t complete_turn_sem;

    sem_t client_shot;
    sem_t game_update;
    sem_t complete_game_update;

    sem_t game_loop;

    sem_t done_ships_array;

    pthread_barrier_t game_start_barrier;

    sem_t turn_sem[PLAYER_NUM];
    sem_t get_ships_array[PLAYER_NUM];

    Player players[PLAYER_NUM];

    char ships_array[MAX_BATTLEFIELD_SIZE * MAX_BATTLEFIELD_SIZE];
} SharedMemory;

SharedMemory* shared_memory_init(int num_players);
void shared_memory_destroy(SharedMemory* shm);

#endif
