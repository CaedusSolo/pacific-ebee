#ifndef CELL_H
#define CELL_H

#include "vector2d.h"
#include "game_states.h"
#include "constants.h"
#include <stdbool.h>

typedef struct {
    Vector2D coordinates;
    enum Ship ship_types[PLAYER_NUM];  // Ship type for each player (indexed by player)
    bool has_ship[PLAYER_NUM];  // Whether each player has a ship on this cell
} Cell;

Cell* Cell_new();
Cell* Cell_new_with_coordinates(Vector2D coordinates);
void Cell_delete(Cell* self);
bool Cell_isEmpty(const Cell* self);  // Check if completely empty (no ships from any player)
bool Cell_hasPlayerShip(const Cell* self, int player_index);  // Check if specific player has ship
Vector2D Cell_getCoordinates(const Cell* self);
void Cell_addShip(Cell* self, void* ship);
void Cell_removeShip(Cell* self, void* ship);

#endif
