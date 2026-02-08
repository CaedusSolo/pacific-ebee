#include "battlefield.h"
#include "player.h"
#include "server_connection.h"
#include "server_game_manager.h"
#include "shared_memory.h"
#include "logger.h"
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <inttypes.h>

#define SCORE_FILENAME "scores.txt"

// Flow:
// Start server connection
// Initialize game manager with the server connection
// Accept player connections
// Fork processes once all players are connected
// Start game loop in each process

// Global pointer for signal handlers
SharedMemory* global_shm = NULL;
FILE* score_file;

void* scheduler_thread(void* arg) {
    SharedMemory* shm = (SharedMemory*)arg;
    while (!shm->game_running) { sleep(1); } // Wait for start (Changed to sleep(1) for warning fix)

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

    if (score_file)
        fclose(score_file);

    exit(0);
}

void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Write players to CSV
void write_scores_to_file(FILE* file, ScoreInfo score_info[], int count) {
    if (file == NULL) return;
    fprintf(file, "name,previous_score,highscore\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%d,%d\n", score_info[i].name,
                score_info[i].previous_score, score_info[i].highscore);
    }
}

// Read players from CSV
int read_from_scores_file(FILE* file, ScoreInfo score_info[]) {
    if (file == NULL) return 0;

    char line[256];
    fgets(line, sizeof(line), file); // Skip header

    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%d,%d",
               score_info[count].name,
               &score_info[count].previous_score,
               &score_info[count].highscore);
        count++;
    }
    return count;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sigint);
    signal(SIGCHLD, handle_sigchld);

    int port = 8999;
    // Dynamic player count handling
    const int num_players = (argc > 2 && argv[2]) ? strtoumax(argv[2], NULL, 10) : 3;
    const int battlefield_size = 5 + (num_players * 5);

    // Initialize Shared Memory
    global_shm = shared_memory_init(num_players);
    if (!global_shm) {
        fprintf(stderr, "Failed to initialize shared memory.\n");
        return 1;
    }

    // Initialize Logger Indices (HEAD)
    global_shm->log_head = 0;
    global_shm->log_tail = 0;

    // Initialize Battlefield Size (Main)
    global_shm->battlefield_size = battlefield_size;

    // Open score file, create if it doesn't exist
    score_file = fopen(SCORE_FILENAME, "r+");
    if (score_file == NULL) {
        score_file = fopen(SCORE_FILENAME, "w");
        fclose(score_file);
        score_file = fopen(SCORE_FILENAME, "r+");
    }

    read_from_scores_file(score_file, global_shm->score_infos);

    // Start Internal Threads (Parent)
    pthread_t log_tid, sched_tid;
    pthread_create(&log_tid, NULL, logger_thread, (void*)global_shm);

    Battlefield battlefield = battlefield_new(battlefield_size, battlefield_size);

    // Network Setup
    int listening_fd = server_connection_start(port);
    printf("[Server] Waiting for %d players on port %d...\n", num_players, port);

    log_event(global_shm, "Server listening on port %d", port);

    // Accept & Fork Loop
    for (int i = 0; i < num_players; i++) {
        int client_fd = server_connection_accept_player(listening_fd);

        if (client_fd < 0) {
            i--;  // try again
            continue;
        }

        // Mark connected in shared memory
        global_shm->players[i].connected = true;

        // Generate ship placement on battlesfield
        battlefield_place_ships_randomly(&battlefield, i);

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        else if (pid == 0) {
            // --- CHILD PROCESS ---
            close(listening_fd);
            Player current_player;
            memset(&current_player, 0, sizeof(Player));
            current_player.fd = client_fd;

            log_event(global_shm, "Player %d connected!", i);

            handle_player(current_player, i, global_shm);
            // pass semaphore details to child process
            exit(0);
        }
        else {
            // --- PARENT PROCESS ---
            // close(client_fd); // Optional: Close in parent if not needed

            global_shm->players[i].pid = pid;
            printf("[Server] Player %d connected (PID: %d)\n", i, pid);
        }
    }

    global_shm->game_running = true;
    printf("[Server] All players connected. Game Starting.\n");

    pthread_create(&sched_tid, NULL, scheduler_thread, (void*)global_shm);
    game_loop(global_shm, &battlefield);

    // Write back the score file
    write_scores_to_file(score_file, global_shm->score_infos, num_players);
    fclose(score_file);
    shared_memory_destroy(global_shm);

    // pthread_join(log_tid, NULL);
    pthread_join(sched_tid, NULL);

    return 0;
}
