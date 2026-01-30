#ifndef MESSAGES_H
#define MESSAGES_H

#include "vector2d.h"
#include "game_states.h"
#include "serializable.h"

struct HitResult : public Serializable<HitResult> {
    enum class Type {
        MISS,
        HIT,
        SINK
    } type;
    Vector2D position;
    Ship shipSunk; // Valid only if type is SINK

    // Serialization implementation
    char* toBytesImpl() const;
    static HitResult fromBytesImpl(const char* data);
};

#endif // !MESSAGES_H
