#include "battlefield.h"
#include "cell.h"
#include "vector2d.h"
#include "game_states.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

Battlefield* battlefield_new(int width, int height) {
    Battlefield* self = (Battlefield*)malloc(sizeof(Battlefield));
    self->width = width;
    self->height = height;

    self->grid = (Cell**)malloc(height * sizeof(Cell*));
    for (int i = 0; i < height; ++i) {
        // iterate each row
        self->grid[i] = (Cell*)malloc(width * sizeof(Cell));
        for (int j = 0; j < width; ++j) {
            // iterate each column
            // since j is each col, so its x, and i is each row so its y
            self->grid[i][j] = *cell_new_with_coordinates(vector2d_create(j, i));
        }
    }
    return self;
}

void battlefield_delete(Battlefield* self) {
    if (self) {
        for (int i = 0; i < self->height; ++i) {
            free(self->grid[i]);
        }
        free(self->grid);
        free(self);
    }
}

Cell* battlefield_get_cell(const Battlefield* self, int x, int y) {
    if (x >= 0 && x < self->width && y >= 0 && y < self->height) {
        return &self->grid[y][x];
    }
    return NULL;
}

int battlefield_get_width(const Battlefield* self) {
    return self->width;
}

int battlefield_get_height(const Battlefield* self) {
    return self->height;
}

void battlefield_update_cell(Battlefield* self, int x, int y, Cell cell) {
    if (x >= 0 && x < self->width && y >= 0 && y < self->height) {
        self->grid[y][x] = cell;
    }
}

// Helper function to get ship size
static int get_ship_size(enum Ship ship) {
    switch(ship) {
        case CARRIER: return 5;
        case BATTLESHIP: return 4;
        case CRUISER: return 3;
        case DESTROYER: return 3;
        case PATROL_BOAT: return 2;
        default: return 0;
    }
}

// Helper function to check if a ship can be placed at a given position for a specific player
static bool can_place_ship(Battlefield* self, int x, int y, int size, bool horizontal, int player_index) {
    // Check bounds
    if (horizontal) {
        if (x + size > self->width || y >= self->height) return false;
    } else {
        if (x >= self->width || y + size > self->height) return false;
    }

    // Check if this player already has a ship in any of these cells
    for (int i = 0; i < size; i++) {
        int check_x = horizontal ? x + i : x;
        int check_y = horizontal ? y : y + i;
        Cell* cell = battlefield_get_cell(self, check_x, check_y);
        if (cell && cell_has_player_ship(cell, player_index)) {
            return false;
        }
    }

    return true;
}

// Helper function to place a ship at a given position for a specific player
static void place_ship(Battlefield* self, int x, int y, int size, bool horizontal,
                       enum Ship ship_type, int player_index) {
    for (int i = 0; i < size; i++) {
        int place_x = horizontal ? x + i : x;
        int place_y = horizontal ? y : y + i;
        Cell* cell = battlefield_get_cell(self, place_x, place_y);
        if (cell) {
            cell->has_ship[player_index] = true;
            cell->ship_types[player_index] = ship_type;
        }
    }
}

// Main function to randomly place all ships for a player
void battlefield_place_ships_randomly(Battlefield* self, int player_index) {
    // Seed random number generator (should be called once per program)
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }

    // Array of all ship types to place
    enum Ship ships[] = {CARRIER, BATTLESHIP, CRUISER, DESTROYER, PATROL_BOAT};
    int num_ships = sizeof(ships) / sizeof(ships[0]);

    // Place each ship
    for (int i = 0; i < num_ships; i++) {
        enum Ship ship = ships[i];
        int size = get_ship_size(ship);
        bool placed = false;
        int max_attempts = 1000;  // Prevent infinite loop

        for (int attempt = 0; attempt < max_attempts && !placed; attempt++) {
            // Random position and orientation
            int x = rand() % self->width;
            int y = rand() % self->height;
            bool horizontal = rand() % 2;

            // Try to place the ship
            if (can_place_ship(self, x, y, size, horizontal, player_index)) {
                place_ship(self, x, y, size, horizontal, ship, player_index);
                placed = true;
            }
        }

        // If we couldn't place a ship after max_attempts, something is wrong
        if (!placed) {
            // Could add error handling here
            return;
        }
    }
}
