#include "connection.h"
#include "messages.h"
#include "vector2d.h"
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>


int connect_to_server(const char* server_ip, int port);


int main (int argc, char *argv[]) {
    // Get the actual first argument (IP address of server)
    // Note that the first argument (argv[0]) is the program name
    const char* serverIP = "127.0.0.1";
    // Default port 8999 if not provided
    const int port = argv[2] ? strtoumax(argv[2], NULL, 10) : 8999;

    int fd = connect_to_server(serverIP, port);
    if (fd < 0) {
        return 1; // Connection failed
    }

    // Print banner

    send_message(fd, argv[1], strlen(argv[1]));

    char buffer[100];
    size_t msg_len;

    while(1) {
        msg_len = listen_for_message(fd, buffer);
        if (buffer[msg_len] != '\0')
            buffer[msg_len + 1] = '\0';


        if (!strcmp(buffer, "GAME_START")) {
            printf("%s\n", buffer);
        }
        else if (!strcmp(buffer, YOUR_TURN)) {
            printf("%s\n", buffer);
            send_message(fd, READY_FOR_TURN, 15);
            int x, y;
            printf("Enter coords: ");
            scanf("%d %d", &x, &y);
            Vector2D pos = vector2d_create(x, y);

            char pos_buffer[8];
            vector2d_serialize(&pos, pos_buffer);
            send_message(fd, pos_buffer, 8);
        }
        else if (!strcmp(buffer, "UPDATE")) {
            printf("%s\n", buffer);
            char update_msg[8];
            size_t update_msg_len;
            listen_for_message(fd, update_msg);
            Vector2D ans = vector2d_deserialize(update_msg);
            printf("Game update: (%d, %d)\n", ans.x, ans.y);
        }
        else if (!strcmp(buffer, "GAME_OVER")) {
            printf("Game finished!\n");
            break;
        }

        memset(buffer, 0, 100);
    }
}

int connect_to_server(const char* server_ip, int port) {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    int status = inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    if (status == 0) {
        perror("Invalid IP address");
        return -1;
    }

    status = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (status < 0) {
        perror("Error connecting to server");
        return -1;
    }

    return sockfd;
}
