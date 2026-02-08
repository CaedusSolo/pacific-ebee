#include "battlefield.h"
#include "cell.h"
#include "vector2d.h"
#include "game_states.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <arpa/inet.h>
#include "game_states.h"

Battlefield battlefield_new(int width, int height) {
    Battlefield self;
    self.width = width;
    self.height = height;

    self.grid = (Cell**)malloc(height * sizeof(Cell*));
    for (int i = 0; i < height; ++i) {
        // iterate each row
        self.grid[i] = (Cell*)malloc(width * sizeof(Cell));
        for (int j = 0; j < width; ++j) {
            // iterate each column
            // since j is each col, so its x, and i is each row so its y
            self.grid[i][j] = *cell_new_with_coordinates(vector2d_create(j, i));
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

void battlefield_to_char_array(const Battlefield* self, int player_index, char* char_array) {
    if (!self || !self->grid || !char_array) return;

    for (int y = 0; y < self->height; y++) {
        for (int x = 0; x < self->width; x++) {
            Cell* cell = &self->grid[y][x];
            char_array[x + (y * self->width)] = cell->has_ship[player_index] ? 'S' : ' ';
        }
    }
}

// Serialize battlefield to provided buffer
// Returns serialized size, or -1 on error
// int battlefield_serialize(const Battlefield* bf, char* buffer) {
//     if (!bf || !bf->grid || !buffer) return -1;
//
//     int cell_size = 8 + (PLAYER_NUM * 4) + PLAYER_NUM;
//     int total_size = 8 + (bf->width * bf->height * cell_size);
//
//     char* ptr = buffer;
//
//     // Width and height
//     *(uint32_t*)ptr = htonl(bf->width);
//     ptr += 4;
//     *(uint32_t*)ptr = htonl(bf->height);
//     ptr += 4;
//
//     // Each cell
//     for (int y = 0; y < bf->height; y++) {
//         for (int x = 0; x < bf->width; x++) {
//             Cell* cell = &bf->grid[y][x];
//
//             *(uint32_t*)ptr = htonl(cell->coordinates.x);
//             ptr += 4;
//             *(uint32_t*)ptr = htonl(cell->coordinates.y);
//             ptr += 4;
//
//             for (int p = 0; p < PLAYER_NUM; p++) {
//                 *(uint32_t*)ptr = htonl((uint32_t)cell->ship_types[p]);
//                 ptr += 4;
//             }
//
//             for (int p = 0; p < PLAYER_NUM; p++) {
//                 *ptr++ = cell->has_ship[p] ? 1 : 0;
//             }
//
//             *ptr = cell->is_shot ? 1 : 0;
//             ptr++;
//         }
//     }
//
//     return total_size;
// }
//
// // Deserialize battlefield from byte buffer
// // Returns 0 on success, -1 on error
// // Deserialize battlefield from buffer (allocates grid)
// // Returns 0 on success, -1 on error
// int battlefield_deserialize(Battlefield* bf, const char* buffer) {
//     const char* ptr = buffer;
//
//     bf->width = ntohl(*(uint32_t*)ptr);
//     ptr += 4;
//     bf->height = ntohl(*(uint32_t*)ptr);
//     ptr += 4;
//
//     int cell_size = 8 + (PLAYER_NUM * 4) + PLAYER_NUM;
//     int expected_size = 8 + (bf->width * bf->height * cell_size);
//
//     // Allocate grid
//     bf->grid = (Cell**)malloc(bf->height * sizeof(Cell*));
//     if (!bf->grid) return -1;
//
//     for (int y = 0; y < bf->height; y++) {
//         bf->grid[y] = (Cell*)malloc(bf->width * sizeof(Cell));
//         if (!bf->grid[y]) {
//             for (int i = 0; i < y; i++) free(bf->grid[i]);
//             free(bf->grid);
//             return -1;
//         }
//     }
//
//     // Deserialize cells
//     for (int y = 0; y < bf->height; y++) {
//         for (int x = 0; x < bf->width; x++) {
//             Cell* cell = &bf->grid[y][x];
//
//             cell->coordinates.x = ntohl(*(uint32_t*)ptr);
//             ptr += 4;
//             cell->coordinates.y = ntohl(*(uint32_t*)ptr);
//             ptr += 4;
//
//             for (int p = 0; p < PLAYER_NUM; p++) {
//                 cell->ship_types[p] = (enum Ship)ntohl(*(uint32_t*)ptr);
//                 ptr += 4;
//             }
//
//             for (int p = 0; p < PLAYER_NUM; p++) {
//                 cell->has_ship[p] = (*ptr++ != 0);
//             }
//
//             cell->is_shot = *ptr;
//         }
//     }
//
//     return 0;
// }
