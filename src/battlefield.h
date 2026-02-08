#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "cell.h"

#define BF_OUR_SHIP 'S'
#define BF_OUR_SHIP_ATTACKED 'a'
#define BF_OUR_ATTACK 'X'
#define BF_OTHER_ATTACK 'o'
#define BF_OTHER_SHIP_ATTACKED 'c'
#define BF_EMPTY ' '


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
void battlefield_to_char_array(const Battlefield* self, int player_index, char* char_array);

#endif
