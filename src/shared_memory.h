#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
#define _XOPEN_SOURCE 700

#include <pthread.h>
#include "constants.h"
#include "player.h"
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

    // Messaging between main process and child processes
    Vector2D shoot_position;

    pthread_mutex_t logger_mutex;
    sem_t log_count_sem;
    pthread_mutex_t game_state_mutex;

    sem_t notify_turn_sem;
    sem_t turn_notified_sem;

    sem_t turn_sem[PLAYER_NUM];
    sem_t change_turn_sem;
    sem_t complete_turn_sem;

    sem_t client_shot;
    sem_t game_update;
    sem_t complete_game_update;

    sem_t game_loop;

    pthread_barrier_t game_start_barrier;

    pid_t player_pids[PLAYER_NUM];
    PlayerName player_names[PLAYER_NUM];
    bool player_connected[PLAYER_NUM];

    int player_scores[PLAYER_NUM];

} SharedMemory;

SharedMemory* shared_memory_init(int num_players);
void shared_memory_destroy(SharedMemory* shm);

#endif
