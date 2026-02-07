#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "cell.h"

typedef struct {
    Cell** grid;  // 2D array of Cells
    int width;
    int height;
} Battlefield;

Battlefield* Battlefield_new(int width, int height);
void Battlefield_delete(Battlefield* self);
Cell* Battlefield_getCell(const Battlefield* self, int x, int y);
int Battlefield_getWidth(const Battlefield* self);
int Battlefield_getHeight(const Battlefield* self);
void Battlefield_updateCell(Battlefield* self, int x, int y, Cell cell);
void Battlefield_placeShipsRandomly(Battlefield* self, int player_index);

#endif
