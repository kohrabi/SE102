#include "Vector2.h"

#include <cmath>

const Vector2 Vector2::Zero = Vector2(0, 0);
const Vector2 Vector2::One = Vector2(1, 1);

Vector2 Vector2::operator+(const Vector2& a) const { return Vector2(x + a.x, y + a.y); }
Vector2 Vector2::operator-(const Vector2& a) const { return Vector2(x - a.x, y - a.y); }
Vector2 Vector2::operator*(const Vector2& a) const { return Vector2(x * a.x, y * a.y); }
Vector2 Vector2::operator/(const Vector2& a) const { return Vector2(x / a.x, y / a.y); }
Vector2& Vector2::operator+=(const Vector2& a) { x += a.x; y += a.y; return *this; }
Vector2& Vector2::operator-=(const Vector2& a) { x -= a.x; y -= a.y; return *this; }
Vector2& Vector2::operator*=(const Vector2& a) { x *= a.x; y *= a.y; return *this; }
Vector2& Vector2::operator/=(const Vector2& a) { x /= a.x; y /= a.y; return *this; }

float Vector2::dot() const {
	return x * x + y * y;
}

void Vector2::normalize()
{
	float leng = length();
	x /= leng;
	y /= leng;
}

Vector2 Vector2::normalizeVector2() const
{
	Vector2 newVector = *this;
	newVector.normalize();
	return newVector;
}

float Vector2::distance(const Vector2& b) const
{
	return (b - *this).length();
}

float Vector2::length() const {
	return sqrt(x * x + y * y);
}

float Vector2::lengthSquared() const
{
	return dot();
}