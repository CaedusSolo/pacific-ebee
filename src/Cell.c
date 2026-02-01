#include "Cell.h"
#include <stdlib.h>

Cell* Cell_new() {
    Cell* self = (Cell*)malloc(sizeof(Cell));
    self->coordinates = Vector2D_new(0, 0);
    return self;
}

Cell* Cell_new_with_coordinates(Vector2D coordinates) {
    Cell* self = (Cell*)malloc(sizeof(Cell));
    self->coordinates = coordinates;
    return self;
}

void Cell_delete(Cell* self) {
    if (self) {
        free(self);
    }
}

Vector2D Cell_getCoordinates(const Cell* self) {
    return self->coordinates;
}

bool Cell_isEmpty(const Cell* self) {
    // Simplified: no ships array yet
    return true;
}

void Cell_addShip(Cell* self, void* ship) {
    // TODO: Implement ship array/list
}

void Cell_removeShip(Cell* self, void* ship) {
    // TODO: Implement ship removal
}
