#include "vector2d.h"

#include <cmath>

using namespace std;



const Vector2D Vector2D::ZERO = Vector2D(0,0);

Vector2D::Vector2D(int x, int y) {
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

Vector2D& Vector2D::operator*=(const int& rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
}

Vector2D operator*(const Vector2D& lhs, const int& rhs) {
    Vector2D result = lhs;
    result *= rhs;
    return result;
}

Vector2D operator*(const int& lhs, const Vector2D& rhs) {
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

int Vector2D::distance(const Vector2D& other) const {
    // In the assignment, it seems to imply taxicab distance function
    // But the initial GenericRobot kinda not follow that ????
    // Imma use normal Pythagorean distance for now

    int diff_squared_x = pow(this->x - other.x, 2);
    int diff_squared_y = pow(this->y - other.y, 2);

    return static_cast<int>(
        sqrt(diff_squared_x + diff_squared_y)
    );
}

int Vector2D::magnitude() const {
    return static_cast<int>(
        sqrt(pow(x, 2) + pow(y, 2))
    );
}

Vector2D Vector2D::normalized() const {
    int m = this->magnitude();
    return Vector2D(
        x / m, y / m
    );
}
