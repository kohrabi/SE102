#include "Vector2i.h"

#include <cmath>

const Vector2i Vector2i::Zero = Vector2i(0, 0);
const Vector2i Vector2i::One = Vector2i(1, 1);

Vector2i Vector2i::operator+(const Vector2i& a) const { return Vector2i(x + a.x, y + a.y); }
Vector2i Vector2i::operator-(const Vector2i& a) const { return Vector2i(x - a.x, y - a.y); }
Vector2i Vector2i::operator*(const Vector2i& a) const { return Vector2i(x * a.x, y * a.y); }
Vector2i Vector2i::operator/(const Vector2i& a) const { return Vector2i(x / a.x, y / a.y); }
Vector2i& Vector2i::operator+=(const Vector2i& a) { x += a.x; y += a.y; return *this; }
Vector2i& Vector2i::operator-=(const Vector2i& a) { x -= a.x; y -= a.y; return *this; }
Vector2i& Vector2i::operator*=(const Vector2i& a) { x *= a.x; y *= a.y; return *this; }
Vector2i& Vector2i::operator/=(const Vector2i& a) { x /= a.x; y /= a.y; return *this; }

float Vector2i::dot() const {
	return x * x + y * y;
}

void Vector2i::normalize()
{
	int leng = length();
	x /= leng;
	y /= leng;
}

Vector2i Vector2i::normalized() const
{
	Vector2i newVector = *this;
	newVector.normalize();
	return newVector;
}

float Vector2i::distance(const Vector2i& b) const
{
	return (b - *this).length();
}

float Vector2i::length() const {
	return sqrt(x * x + y * y);
}

float Vector2i::lengthSquared() const
{
	return dot();
}