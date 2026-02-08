#include "logger.h"
#include "constants.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void *logger_thread(void *arg) {
    SharedMemory* shm = (SharedMemory*)arg;

    FILE* lf = fopen("game.log", "w");
    if (!lf) {
        perror("Failed to open game.log");
        return NULL;
    }

    fprintf(lf, "----SERVER STARTED----\n");
    fflush(lf);

    while (1) {
        sem_wait(&shm->log_count_sem);  // wait for signal
        pthread_mutex_lock(&shm->logger_mutex); // lock

        // Process all pending messages
        while (shm->log_head != shm->log_tail) {
            char *msg = shm->log_queue[shm->log_head].message;

            fprintf(lf, "%s\n", msg);

            shm->log_head = (shm->log_head + 1) % LOG_QUEUE_SIZE;
        }

        fflush(lf); // Ensure writes are flushed to disk
        pthread_mutex_unlock(&shm->logger_mutex);
    }
    fclose(lf);
    return NULL;
}

void log_event(SharedMemory *shm, const char* format, ...) {
    if (!shm) return;

    char buffer[MAX_LOG_LENGTH];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    pthread_mutex_lock(&shm->logger_mutex);

    int next = (shm->log_tail + 1) % LOG_QUEUE_SIZE;

    if (next != shm->log_head) {
        strncpy(shm->log_queue[shm->log_tail].message, buffer, MAX_LOG_LENGTH - 1);
        shm->log_queue[shm->log_tail].message[MAX_LOG_LENGTH - 1] = '\0';
        shm->log_tail = next;
        sem_post(&shm->log_count_sem);
    } else {
        fprintf(stderr, "[Logger] Queue is full! Dropped: %s\n", buffer);
    }

    pthread_mutex_unlock(&shm->logger_mutex);
}
