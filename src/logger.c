#include "logger.h"
#include "constants.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

void *logger_thread(void *arg) {
    SharedMemory* shm = (SharedMemory*)arg;

    FILE* lf = fopen("game.log", "a"); // append mode
    if (!lf) {
        perror("Failed to open game.log");
        return NULL;
    }

    fprintf(lf, "----SERVER STARTED----\n");
    fflush(lf);

    while (true) {
        sem_wait(&shm->log_count_sem);  // wait for signal
        pthread_mutex_lock(&shm->logger_mutex); // lock

        if (shm->log_head != shm_log_tail) {
            char *msg = shm->log_queue[shm->log_head].message;

            time_t now = time(NULL);
            struct tm* time = localtime(&now);
            char time_str[64];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

            fprintf(lf, "[%s] %s\n", time_str, msg);
            fflush(lf);

            shm->log_head = (shm->log_head + 1) % LOG_QUEUE_SIZE;
        }
        pthread_mutex_unlock(&shm->logger_mutex);
    }
    fclose(lf);
    return NULL;
}

void log_event(SharedMemory *shm, const char* format) {
    if (!shm) return;

    char buffer[MAX_LOG_LENGTH];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // lock mutex to block
    pthread_mutex_lock(&shm->logger_mutex);

    if (next != shm->log_head) {
        // copy message to shared memory
        strncpy(shm->log_queue[shm->log_tail].message, buffer, MAX_LOG_LENGTH - 1);
        shm->log_queue[shm->log_tail].message[MAX_LOG_LENGTH - 1] = '\0';
        shm->log_tail = next;  // move tail

        sem_post(&shm->log_count_sem);
    } else {
        fprintf(stderr, "[Logger] Queue is full! Message: %s\n", buffer)
    }

    // unlock mutex after done
    pthread_mutex_unlock(&shm->logger_mutex);

}
