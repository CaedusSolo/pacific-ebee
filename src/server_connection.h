#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "connection.h"

int server_connection_start(const int port_number);
int acceptPlayer(const int fd);

#endif
