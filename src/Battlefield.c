#include "Battlefield.h"
#include "Cell.h"
#include "vector2d.h"
#include <stdlib.h>

Battlefield* Battlefield_new(int width, int height) {
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
            self->grid[i][j] = *Cell_new_with_coordinates(Vector2D_new(j, i));
        }
    }
    return self;
}

void Battlefield_delete(Battlefield* self) {
    if (self) {
        for (int i = 0; i < self->height; ++i) {
            free(self->grid[i]);
        }
        free(self->grid);
        free(self);
    }
}

Cell* Battlefield_getCell(const Battlefield* self, int x, int y) {
    if (x >= 0 && x < self->width && y >= 0 && y < self->height) {
        return &self->grid[y][x];
    }
    return NULL;
}

int Battlefield_getWidth(const Battlefield* self) {
    return self->width;
}

int Battlefield_getHeight(const Battlefield* self) {
    return self->height;
}

void Battlefield_updateCell(Battlefield* self, int x, int y, Cell cell) {
    if (x >= 0 && x < self->width && y >= 0 && y < self->height) {
        self->grid[y][x] = cell;
    }
}
