#include "server_connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int server_connection_start(const int port_number) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_number);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d ...\n", port_number);
    return server_fd;
}

int server_connection_accept_player(const int fd) {
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    printf("Waiting for connection...\n");

    int newFD = accept(fd, (struct sockaddr*)&clientAddr, (socklen_t*)&addrLen);

    if (newFD < 0) {
        perror("Accept failed. Aborting now.");
        return -1;
    }

    return newFD;
}
