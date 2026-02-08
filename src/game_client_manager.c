#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_client_manager.h"
#include "connection.h"
#include "renderer.h"
#include "messages.h"
#include "vector2d.h"


void handle_game_start(GameClientData* game_data) {
    // Get battlefield
    listen_for_message(game_data->fd, game_data->ships_board);
    printf("Battlefield acquired!\n");

    char player_count_buffer[4];
    listen_for_message(game_data->fd, player_count_buffer);
    int player_count = int_deserialize(player_count_buffer);
    game_data->player_count = player_count;

    char player_names_buffer[256*game_data->player_count];
    listen_for_message(game_data->fd, player_names_buffer);
    for (int i = 0; i < game_data->player_count; i++) {
        memcpy(game_data->player_names[i], player_names_buffer + (256*i), 256);
    }
    printf("Player names acquired!\n");

    char battlefield_size_buffer[4];
    listen_for_message(game_data->fd, battlefield_size_buffer);
    int battlefield_size = int_deserialize(battlefield_size_buffer);
    game_data->battlefield_size = battlefield_size;
    printf("Battlefield size acquired: %d\n", game_data->battlefield_size);

    grid(game_data->battlefield_size, game_data->ships_board, true);
}

void handle_turn(GameClientData* game_data) {
    int fd = game_data->fd;

    send_message(fd, READY_FOR_TURN, 15);
    int x, y;
    printf("Enter coords: ");
    scanf("%d %d", &x, &y);
    Vector2D pos = vector2d_create(x, y);

    char pos_buffer[8];
    vector2d_serialize(&pos, pos_buffer);
    send_message(fd, pos_buffer, 8);

    game_data->attacks_board[coord_to_index(pos, game_data->battlefield_size)] = 'O';
}

void handle_game_update(GameClientData* game_data) {
    int fd = game_data->fd;

    char update_msg[272];
    size_t update_msg_len;
    listen_for_message(fd, update_msg);
    HitResult hr;
    hitresult_deserialize(&hr, update_msg);
    printf("Game update!\n");
    hitresult_print(&hr);
}

int coord_to_index(Vector2D vector2d, int width) {
    return vector2d.x + (vector2d.y * width);
}
