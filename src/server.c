#include "battlefield.h"
#include "connection.h"
#include "constants.h"
#include "player.h"
#include "server_connection.h"
#include "server_game_manager.h"
#include "shared_memory.h"
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

// Flow:
// Start server connection
// Initialize game manager with the server connection
// Accept player connections
// Fork processes once all players are connected
// Start game loop in each process

// Global pointer for signal handlers
SharedMemory* global_shm = NULL;

void* logger_thread(void* arg) {
    SharedMemory* shm = (SharedMemory*)arg;
    while (1) {
        sem_wait(&shm->log_count_sem);
        // ... Log writing logic ...
        // printf("Logger thread woke up\n");
    }
    return NULL;
}

void* scheduler_thread(void* arg) {
    SharedMemory* shm = (SharedMemory*)arg;
    while (!shm->game_running) { sleep(100000); } // Wait for start

    printf("[Scheduler] Game running. Managing turns...\n");
    while (!shm->is_game_over) {
        int current = shm->current_player_index;

        // Signal current player
        printf("[Scheduler] Signaling Player %d\n", current);
        sem_post(&shm->turn_sem[current]);

        // Signal main process on turn change
        sem_post(&shm->change_turn_sem);

        // Wait for turn completion
        sem_wait(&shm->complete_turn_sem);

        // Next turn logic (Round Robin)
        shm->current_player_index = (current + 1) % shm->player_num;
    }

    // End the game
    for (int i = 0; i < shm->player_num; i++) {
        sem_post(&shm->turn_sem[i]);
    }

    return NULL;
}

// --- Signal Handlers ---
void handle_sigint(int sig) {
    printf("\n[Server] Shutting down...\n");
    if (global_shm) {
        shared_memory_destroy(global_shm);
    }
    exit(0);
}

void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sigint);
    signal(SIGCHLD, handle_sigchld);

    int port = 8999;
    int num_players = 3;

    // Initialize Shared Memory
    global_shm = shared_memory_init(num_players);
    if (!global_shm) {
        fprintf(stderr, "Failed to initialize shared memory.\n");
        return 1;
    }

    // Start Internal Threads (Parent)
    pthread_t log_tid, sched_tid;
    // pthread_create(&log_tid, NULL, logger_thread, (void*)global_shm);

    Battlefield battlefield = battlefield_new(BATTLEFIELD_SIZE, BATTLEFIELD_SIZE);
    global_shm->battlefield = battlefield;

    // Network Setup
    int listening_fd = server_connection_start(port);
    printf("[Server] Waiting for %d players on port %d...\n", num_players, port);

    // Accept & Fork Loop
    for (int i = 0; i < num_players; i++) {
        int client_fd = server_connection_accept_player(listening_fd);

        if (client_fd < 0) {
            i--;  // try again
            continue;
        }

        // Mark connected in shared memory
        global_shm->player_connected[i] = true;

        // Generate ship placement on battlesfield
        battlefield_place_ships_randomly(&global_shm->battlefield, i);

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        else if (pid == 0) {
            close(listening_fd);
            Player current_player;
            current_player.fd = client_fd;

            handle_player(current_player, i, global_shm);
            // pass semaphore details to child process
            exit(0);
        }
        else {
            // --- PARENT PROCESS ---
            close(client_fd);

            global_shm->player_pids[i] = pid;
            printf("[Server] Player %d connected (PID: %d)\n", i, pid);
        }
    }

    global_shm->game_running = true;
    printf("[Server] All players connected. Game Starting.\n");
    pthread_create(&sched_tid, NULL, scheduler_thread, (void*)global_shm);
    game_loop(global_shm, &battlefield);

    // pthread_join(log_tid, NULL);
    pthread_join(sched_tid, NULL);

    return 0;
}
