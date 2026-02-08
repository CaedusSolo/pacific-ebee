#include "shared_memory.h"
#include "constants.h"
#include "player.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

// Initialize the Shared Memory Block
SharedMemory* shared_memory_init(int num_players) {
    size_t size = sizeof(SharedMemory);
    SharedMemory* shm = mmap(NULL, size, PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shm == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }

    memset(shm, 0, size);
    shm->player_num = num_players;
    shm->game_running = false;
    shm->current_player_index = 0;

    memset(shm->score_infos, 0, num_players * sizeof(ScoreInfo));
    memset(shm->players, 0, num_players * sizeof(Player));

    // 2. Initialize Process-Shared Attributes
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);

    pthread_barrierattr_t battr;
    pthread_barrierattr_init(&battr);
    pthread_barrierattr_setpshared(&battr, PTHREAD_PROCESS_SHARED);

    // 3. Init Synchronization Primitives
    pthread_mutex_init(&shm->logger_mutex, &mattr);
    pthread_mutex_init(&shm->game_state_mutex, &mattr);

    // Semaphores (The '1' means shared between processes)
    sem_init(&shm->log_count_sem, 1, 0);
    sem_init(&shm->complete_turn_sem, 1, 0);
    sem_init(&shm->change_turn_sem, 1, 0);
    sem_init(&shm->notify_turn_sem, 1, 0);
    sem_init(&shm->turn_notified_sem, 1, 0);
    sem_init(&shm->client_shot, 1, 0);
    sem_init(&shm->game_update, 1, 0);
    sem_init(&shm->complete_game_update, 1, 0);
    sem_init(&shm->game_loop, 1, 0);
    sem_init(&shm->done_ships_array, 1, 0);

    for (int i = 0; i < MAX_PLAYER_NUM; i++) {
        sem_init(&shm->turn_sem[i], 1, 0);
        sem_init(&shm->get_ships_array[i], 1, 0);
    }

    pthread_barrier_init(&shm->game_start_barrier, &battr, num_players);

    // cleanup
    pthread_mutexattr_destroy(&mattr);
    pthread_barrierattr_destroy(&battr);

    return shm;
}

void shared_memory_destroy(SharedMemory* shm) {
    if (!shm) return;

    pthread_mutex_destroy(&shm->logger_mutex);
    pthread_mutex_destroy(&shm->game_state_mutex);
    pthread_barrier_destroy(&shm->game_start_barrier);

    sem_destroy(&shm->log_count_sem);
    sem_destroy(&shm->complete_turn_sem);
    for(int i=0; i<MAX_PLAYER_NUM; i++) sem_destroy(&shm->turn_sem[i]);

    munmap(shm, sizeof(SharedMemory));
}
