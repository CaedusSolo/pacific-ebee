#ifndef CELL_H
#define CELL_H

#include "vector2d.h"

typedef struct {
    Vector2D coordinates;
    // Note: Ship vector/array would need to be defined
    // For now, this is a simplified structure
} Cell;

Cell* Cell_new();
Cell* Cell_new_with_coordinates(Vector2D coordinates);
void Cell_delete(Cell* self);
bool Cell_isEmpty(const Cell* self);
Vector2D Cell_getCoordinates(const Cell* self);
void Cell_addShip(Cell* self, void* ship);
void Cell_removeShip(Cell* self, void* ship);

#endif
