#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <iostream>
#include <ostream>

using namespace std;

class Vector2D {
public:
    int x;
    int y;

    static const Vector2D ZERO;
    
    Vector2D(int x_val = 0, int y_val = 0);
    
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);

    Vector2D& operator*=(const int& rhs);
    friend Vector2D operator*(const Vector2D& lhs, const int& rhs);
    friend Vector2D operator*(const int& lhs, const Vector2D& rhs);

    // Magnitude of the vector
    int magnitude() const;

    // Returns the unit vector version of a vector2d
    // Which is essentially the 'direction' part of a vector
    Vector2D normalized() const;

    // This function doesn't accept 'self' parameter, so it must not be a member function
    // Putting it as friend function isn't technically needed; operator== doesn't need private members anyway
    // it's simply just to imply that it is related to this class
    friend bool operator==(const Vector2D& a, const Vector2D& b);
    friend bool operator!=(const Vector2D& a, const Vector2D& b);

    friend ostream& operator<<(ostream& stream, const Vector2D& vec);

    int distance(const Vector2D& other) const;
};

#endif // !VECTOR_2D_H
