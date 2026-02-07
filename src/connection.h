#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdlib.h>

void send_message(const int fd, const char* outgoingMessage, int len);
size_t listen_for_message(const int fd, char* msg);

#endif
