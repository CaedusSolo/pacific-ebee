#ifndef CONNECTION_H
#define CONNECTION_H


void send_message(const int fd, const char* outgoingMessage);
char* listen_for_message(const int fd);

#endif
