#ifndef CONNECTION_H
#define CONNECTION_H


void sendMessage(const int fd, const char* outgoingMessage);
char* listenForMessage(const int fd);

#endif
