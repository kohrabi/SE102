#pragma once
#include <ostream>

class Vector2 {
public:
	float x;
	float y;

	static const Vector2 Zero;
	static const Vector2 One;
	
	Vector2() : x(0), y(0) {}
	Vector2(float _xy) : x(_xy), y(_xy) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	Vector2& operator=(const Vector2& a) { x = a.x; y = a.y; return *this; }
	Vector2 operator+(const Vector2& a) const;
	Vector2 operator-(const Vector2& a) const;
	Vector2 operator*(const Vector2& a) const;
	Vector2 operator/(const Vector2& a) const;
	Vector2& operator+=(const Vector2& a);
	Vector2& operator-=(const Vector2& a);
	Vector2& operator*=(const Vector2& a);
	Vector2& operator/=(const Vector2& a);

	float length() const;
	float lengthSquared() const;
	float dot() const;
	void normalize();
	Vector2 normalized() const;

	float distance(const Vector2& b) const;

	bool operator==(const Vector2& vector) const { return x == vector.x && y == vector.y; }
	bool operator!=(const Vector2& vector) const { return x != vector.x || y != vector.y; }
};

