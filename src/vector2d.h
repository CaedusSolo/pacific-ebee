#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Vector2D {
    int x;
    int y;
} Vector2D;

// Constants
extern const Vector2D ZERO;

// Constructor-like function
Vector2D vector2d_create(int x_val, int y_val);

// Arithmetic operations (replacing operator+, operator-)
Vector2D vector2d_add(const Vector2D* a, const Vector2D* b);
Vector2D vector2d_sub(const Vector2D* a, const Vector2D* b);

// In-place operations (replacing operator+=, operator-=)
void vector2d_add_assign(Vector2D* self, const Vector2D* other);
void vector2d_sub_assign(Vector2D* self, const Vector2D* other);

// Scalar multiplication (replacing operator*)
void vector2d_mul_assign(Vector2D* self, int scalar);
Vector2D vector2d_mul(const Vector2D* vec, int scalar);

// Magnitude of the vector
int vector2d_magnitude(const Vector2D* self);

// Returns the unit vector version of a vector2d
// Which is essentially the 'direction' part of a vector
Vector2D vector2d_normalized(const Vector2D* self);

// Comparison operations (replacing operator==, operator!=)
bool vector2d_equals(const Vector2D* a, const Vector2D* b);
bool vector2d_not_equals(const Vector2D* a, const Vector2D* b);

// Distance calculation
int vector2d_distance(const Vector2D* self, const Vector2D* other);

// Serialization implementation
void vector2d_serialize(const Vector2D* self, char *buffer);
Vector2D vector2d_deserialize(const char *buffer);

#endif // !VECTOR_2D_H
