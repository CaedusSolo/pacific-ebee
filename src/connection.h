#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>

void send_message(const int fd, const char* outgoingMessage);
char* listen_for_message(const int fd, size_t* output_len);

#endif
