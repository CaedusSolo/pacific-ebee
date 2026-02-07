#include "vector2d.h"

#include <netinet/in.h>
#include <stdint.h>
#include <arpa/inet.h>  // for htonl/ntohl
#include <math.h>


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

    double diff_squared_x = pow((double)self->x - (double)other->x, 2.0);
    double diff_squared_y = pow((double)self->y - (double)other->y, 2.0);

    return (int)sqrt(diff_squared_x + diff_squared_y);
}

int vector2d_magnitude(const Vector2D* self) {
    return (int)sqrt(pow((double)self->x, 2.0) + pow((double)self->y, 2.0));
}

Vector2D vector2d_normalized(const Vector2D* self) {
    int m = vector2d_magnitude(self);
    return vector2d_create(self->x / m, self->y / m);
}

// Serialize two uint32_t into byte array (network byte order)
void vector2d_serialize(const Vector2D* self, char *buffer) {
    uint32_t net_x = htonl(self->x);
    uint32_t net_y = htonl(self->y);

    // Copy to buffer
    for (int i = 0; i < 4; i++) {
        buffer[i] = (net_x >> (24 - i * 8)) & 0xFF;
        buffer[i + 4] = (net_y >> (24 - i * 8)) & 0xFF;
    }
}

// Deserialize byte array into two uint32_t (from network byte order)
Vector2D vector2d_deserialize(const char *buffer) {
    uint32_t net_x = 0;
    uint32_t net_y = 0;

    // Read from buffer
    for (int i = 0; i < 4; i++) {
        net_x |= ((uint32_t)buffer[i]) << (24 - i * 8);
        net_y |= ((uint32_t)buffer[i + 4]) << (24 - i * 8);
    }

    return vector2d_create(ntohl(net_x), ntohl(net_y));
}
