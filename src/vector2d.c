#include "vector2d.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>


const Vector2D ZERO = {0, 0};

Vector2D vector2d_create(int x_val, int y_val) {
    Vector2D vec;
    vec.x = x_val;
    vec.y = y_val;
    return vec;
}

Vector2D vector2d_add(const Vector2D* a, const Vector2D* b) {
    return vector2d_create(a->x + b->x, a->y + b->y);
}

Vector2D vector2d_sub(const Vector2D* a, const Vector2D* b) {
    return vector2d_create(a->x - b->x, a->y - b->y);
}

void vector2d_add_assign(Vector2D* self, const Vector2D* other) {
    self->x += other->x;
    self->y += other->y;
}

void vector2d_sub_assign(Vector2D* self, const Vector2D* other) {
    self->x -= other->x;
    self->y -= other->y;
}

void vector2d_mul_assign(Vector2D* self, int scalar) {
    self->x *= scalar;
    self->y *= scalar;
}

Vector2D vector2d_mul(const Vector2D* vec, int scalar) {
    Vector2D result = *vec;
    vector2d_mul_assign(&result, scalar);
    return result;
}

bool vector2d_equals(const Vector2D* a, const Vector2D* b) {
    return (a->x == b->x) && (a->y == b->y);
}

bool vector2d_not_equals(const Vector2D* a, const Vector2D* b) {
    return !vector2d_equals(a, b);
}

int vector2d_distance(const Vector2D* self, const Vector2D* other) {
    // In the assignment, it seems to imply taxicab distance function
    // But the initial GenericRobot kinda not follow that ????
    // Imma use normal Pythagorean distance for now

    int diff_squared_x = pow(self->x - other->x, 2);
    int diff_squared_y = pow(self->y - other->y, 2);

    return (int)sqrt(diff_squared_x + diff_squared_y);
}

int vector2d_magnitude(const Vector2D* self) {
    return (int)sqrt(pow(self->x, 2) + pow(self->y, 2));
}

Vector2D vector2d_normalized(const Vector2D* self) {
    int m = vector2d_magnitude(self);
    return vector2d_create(self->x / m, self->y / m);
}
