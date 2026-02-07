#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "cell.h"

typedef struct {
    int width;
    int height;
    Cell** grid;  // 2D array of Cells
} Battlefield;

Battlefield battlefield_new(int width, int height);
void battlefield_delete(Battlefield* self);
Cell* battlefield_get_cell(const Battlefield* self, int x, int y);
int battlefield_get_width(const Battlefield* self);
int battlefield_get_height(const Battlefield* self);
void battlefield_update_cell(Battlefield* self, int x, int y, Cell cell);
void battlefield_place_ships_randomly(Battlefield* self, int player_index);
int battlefield_serialize(const Battlefield* bf, char* buffer);
int battlefield_deserialize(Battlefield* bf, const char* buffer);

#endif
