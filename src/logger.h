#ifndef LOGGER.H
#define LOGGER.H

#include "shared_memory.h"

void *logger_thread(void *arg); // logger thread loop

void log_event(SharedMemory *shm, const char* format);


#endif
