#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include "connection.h"

int server_connection_start(const int port_number);
int server_connection_accept_player(const int fd);

#endif
