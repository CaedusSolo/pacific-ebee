#ifndef MESSAGES_H
#define MESSAGES_H

#include "vector2d.h"
#include "game_states.h"

typedef struct HitResult {
    enum Type {
        MISS,
        HIT,
        SINK
    } type;
    Vector2D position;
    enum Ship ship_sunk; // Valid only if type is SINK
} HitResult;

#endif // !MESSAGES_H
