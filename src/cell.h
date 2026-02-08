#ifndef CELL_H
#define CELL_H

#include "vector2d.h"
#include "game_states.h"
#include "constants.h"
#include <stdbool.h>

typedef struct {
    Vector2D coordinates;
    enum Ship ship_types[MAX_PLAYER_NUM];  // Ship type for each player (indexed by player)
    bool has_ship[MAX_PLAYER_NUM];  // Whether each player has a ship on this cell
    bool is_shot;
} Cell;

Cell* cell_new();
Cell* cell_new_with_coordinates(Vector2D coordinates);
void cell_delete(Cell* self);
bool cell_is_empty(const Cell* self);  // Check if completely empty (no ships from any player)
bool cell_has_player_ship(const Cell* self, int player_index);  // Check if specific player has ship
Vector2D cell_get_coordinates(const Cell* self);
void cell_add_ship(Cell* self, void* ship);
void cell_remove_ship(Cell* self, void* ship);

#endif
