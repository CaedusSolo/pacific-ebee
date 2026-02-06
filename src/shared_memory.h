#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
#define _XOPEN_SOURCE 700

#include <pthread.h>
#include "constants.h"
#include "vector2d.h"
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

    pthread_mutex_t logger_mutex;
    sem_t log_count_sem;
    pthread_mutex_t game_state_mutex;
    sem_t turn_sem[PLAYER_NUM];
    sem_t complete_turn_sem;

    pthread_barrier_t game_start_barrier;

    char board[PLAYER_NUM][BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];
    pid_t player_pids[PLAYER_NUM];
    char player_names[PLAYER_NUM][256];
    bool player_connected[PLAYER_NUM];

    int player_scores[PLAYER_NUM];

} SharedMemory;

SharedMemory* shared_memory_init(int num_players);
void shared_memory_destroy(SharedMemory* shm);

#endif
