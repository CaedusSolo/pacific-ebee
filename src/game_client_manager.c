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
    listen_for_message(game_data->fd, game_data->game_board);
    printf("Battlefield acquired!\n");

    char player_count_buffer[4];
    listen_for_message(game_data->fd, player_count_buffer);
    int player_count = int_deserialize(player_count_buffer);
    game_data->player_count = player_count;
    printf("Player count acquired!\n");

    char our_index_buffer[4];
    listen_for_message(game_data->fd, our_index_buffer);
    int our_index = int_deserialize(our_index_buffer);
    game_data->our_index = our_index;
    printf("Our index acquired!\n");

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

    grid(game_data->battlefield_size, game_data->game_board);
}

int letter_to_number(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 1;  // A=1, B=2, ..., Z=26
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 27;  // a=27, b=28, ..., z=52
    }
    return -1;  // Invalid character
}

void handle_turn(GameClientData* game_data) {
    int fd = game_data->fd;

    send_message(fd, READY_FOR_TURN, 15);
    int y, x; char x_char;
    printf("Enter coords: ");
    scanf("%c %d", &x_char, &y);
    x = letter_to_number(x_char);
    // Minus 1 to turn into indices
    Vector2D pos = vector2d_create(x - 1, y - 1);

    char pos_buffer[8];
    vector2d_serialize(&pos, pos_buffer);
    send_message(fd, pos_buffer, 8);
    client_wait_for_hit_result(game_data);
}

void handle_game_update(GameClientData* game_data) {
    client_wait_for_hit_result(game_data);
    grid(game_data->battlefield_size, game_data->game_board);
}

void handle_game_over(GameClientData* game_data) {
    char score_buffer[4];
    listen_for_message(game_data->fd, score_buffer);
    game_data->score = int_deserialize(score_buffer);
    printf("Your score: %d\n", game_data->score);
}

int coord_to_index(Vector2D vector2d, int width) {
    return vector2d.x + (vector2d.y * width);
}

void client_wait_for_hit_result(GameClientData* game_data) {
    char update_msg[272];
    listen_for_message(game_data->fd, update_msg);
    HitResult hr;
    hitresult_deserialize(&hr, update_msg);

    char new_value;
    char* attacker_name;
    char* victim_name;
    char msg[1000];

    if (hr.attacker_index != game_data->our_index)
        attacker_name = game_data->player_names[hr.attacker_index];
    else
        attacker_name = "You";

    if (hr.victim_index != game_data->our_index && hr.type != MISS)
        victim_name = game_data->player_names[hr.victim_index];
    else
        victim_name = "your";

    switch (hr.type) {
        case MISS:
            if (hr.attacker_index == game_data->our_index)
                new_value = BF_OUR_ATTACK;
            else
                new_value = BF_OTHER_ATTACK;
            sprintf(msg, "%s missed!\n", attacker_name);
            break;

        case HIT:
            if (hr.victim_index == game_data->our_index)
                new_value = BF_OUR_SHIP_ATTACKED;
            else
                new_value = BF_OTHER_SHIP_ATTACKED;
            sprintf(msg, "%s hit %s ship!\n", attacker_name, victim_name);
            break;
    }

    game_client_data_set_board_char(game_data, hr.position, new_value);
    grid(game_data->battlefield_size, game_data->game_board);
    printf("%s\n", msg);
}

void game_client_data_set_board_char(GameClientData* self, Vector2D pos, char val) {
    self->game_board[coord_to_index(pos, self->battlefield_size)] = val;
}
