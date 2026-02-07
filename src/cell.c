#include "cell.h"
#include <stdlib.h>

Cell* cell_new() {
    Cell* self = (Cell*)malloc(sizeof(Cell));
    self->coordinates = vector2d_create(0, 0);
    for (int i = 0; i < PLAYER_NUM; i++) {
        self->has_ship[i] = false;
        self->ship_types[i] = CARRIER;  // default, irrelevant when has_ship is false
    }
    return self;
}

Cell* cell_new_with_coordinates(Vector2D coordinates) {
    Cell* self = (Cell*)malloc(sizeof(Cell));
    self->coordinates = coordinates;
    for (int i = 0; i < PLAYER_NUM; i++) {
        self->has_ship[i] = false;
        self->ship_types[i] = CARRIER;  // default, irrelevant when has_ship is false
    }
    return self;
}

void cell_delete(Cell* self) {
    if (self) {
        free(self);
    }
}

Vector2D cell_get_coordinates(const Cell* self) {
    return self->coordinates;
}

bool cell_is_empty(const Cell* self) {
    // Check if completely empty (no ships from any player)
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (self->has_ship[i]) {
            return false;
        }
    }
    return true;
}

bool cell_has_player_ship(const Cell* self, int player_index) {
    if (player_index >= 0 && player_index < PLAYER_NUM) {
        return self->has_ship[player_index];
    }
    return false;
}

void cell_add_ship(Cell* self, void* ship) {
    // TODO: Implement ship array/list
}

void cell_remove_ship(Cell* self, void* ship) {
    // TODO: Implement ship removal
}
