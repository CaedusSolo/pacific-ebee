#include "connection.h"
#include "player.h"
#include "server_connection.h"
#include "server_game_manager.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


// Flow:
// Start server connection
// Initialize game manager with the server connection
// Accept player connections
// Fork processes once all players are connected
// Start game loop in each process

int main (int argc, char *argv[]) {
    // Default port
    int port = 8999;
    int listening_fd = server_connection_start(port);

    int num_players = 3;
    ServerGameManager game_manager = server_game_manager_create(num_players, listening_fd);

    for(int i = 0;i<num_players;i++){
        int conn_fd = server_connection_accept_player(listening_fd);

        Player new_player;
        new_player.fd = conn_fd;

        // Client should send player name upon connection
        size_t player_name_length;
        char *player_name_heap_ptr = listen_for_message(new_player.fd, &player_name_length);
        strncpy(new_player.name, player_name_heap_ptr, player_name_length);
        new_player.name[player_name_length] = '\0';

        game_manager.players[i] = new_player;
    }

    close(listening_fd);

    // Close the listening socker before forking
    // Avoid child processes listening on the same socket
    for (int i = 0; i < num_players; i++) {
        game_manager.current_player_index = i;
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            handle_player(game_manager.players[i]);
            return 0;
        }
        else {
            printf("Server notification: Player %s connected\n", game_manager.players[i].name);
        }
    }

    // Start game loop
    game_loop(&game_manager);

    return 0;
}
