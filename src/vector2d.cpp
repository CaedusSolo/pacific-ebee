#include "vector2d.h"

#include <cmath>
#include <cstring>

using namespace std;



const Vector2D Vector2D::ZERO = Vector2D(0,0);

Vector2D::Vector2D(uint32_t x, uint32_t y) {
    this->x = x;
    this->y = y;
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    this->x += other.x;
    this->y += other.y;

    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    this->x -= other.x;
    this->y -= other.y;

    return *this;
}

Vector2D& Vector2D::operator*=(const uint32_t& rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
}

Vector2D operator*(const Vector2D& lhs, const uint32_t& rhs) {
    Vector2D result = lhs;
    result *= rhs;
    return result;
}

Vector2D operator*(const uint32_t& lhs, const Vector2D& rhs) {
    Vector2D result = rhs;
    result *= lhs;
    return result;
}

bool operator==(const Vector2D& a, const Vector2D& b) {
    return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(const Vector2D& a, const Vector2D& b) {
    return !(a == b);
}

ostream& operator<<(ostream& stream, const Vector2D& vec) {
    stream << "( x: " << vec.x << ", y: " << vec.y << " )";
    return stream;
}

uint32_t Vector2D::distance(const Vector2D& other) const {
    // In the assignment, it seems to imply taxicab distance function
    // But the initial GenericRobot kinda not follow that ????
    // Imma use normal Pythagorean distance for now

    uint32_t diff_squared_x = pow(this->x - other.x, 2);
    uint32_t diff_squared_y = pow(this->y - other.y, 2);

    return static_cast<uint32_t>(
        sqrt(diff_squared_x + diff_squared_y)
    );
}

uint32_t Vector2D::magnitude() const {
    return static_cast<uint32_t>(
        sqrt(pow(x, 2) + pow(y, 2))
    );
}

Vector2D Vector2D::normalized() const {
    uint32_t m = this->magnitude();
    return Vector2D(
        x / m, y / m
    );
}

char* Vector2D::toBytesImpl() const {
    char* xBytes = uint32ToBytes(x);
    char* yBytes = uint32ToBytes(y);

    char* bytes = new char[8]; // 4 bytes for x, 4 bytes for y

    std::memcpy(bytes, xBytes, 4);
    std::memcpy(bytes + 4, yBytes, 4);

    delete[] xBytes;
    delete[] yBytes;

    return bytes;
}

Vector2D Vector2D::fromBytesImpl(const char* data) {
    uint32_t x = bytesToUint32(data);
    uint32_t y = bytesToUint32(data + 4);

    return Vector2D(x, y);
}
