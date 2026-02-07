#ifndef MESSAGES_H
#define MESSAGES_H

#include "vector2d.h"
#include "game_states.h"

#define YOUR_TURN "YOUR_TURN"
#define READY_FOR_TURN "READY_FOR_TURN"
#define GAME_START "GAME_START"
#define GAME_UPDATE "GAME_UPDATE"
#define GAME_OVER "GAME_OVER"

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
