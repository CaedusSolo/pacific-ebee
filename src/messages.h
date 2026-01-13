#ifndef MESSAGES_H
#define MESSAGES_H

#include "vector2d.h"
#include "game_states.h"

struct HitResult {
    enum class Type {
        MISS,
        HIT,
        SINK
    } type;
    Vector2D position;
    Ship shipSunk; // Valid only if type is SINK
};

#endif // !MESSAGES_H
