#pragma once
#include <ostream>

class Vector2i {
public:
	int x;
	int y;

	static const Vector2i Zero;
	static const Vector2i One;
	
	Vector2i() : x(0), y(0) {}
	Vector2i(int _xy) : x(_xy), y(_xy) {}
	Vector2i(int _x, int _y) : x(_x), y(_y) {}

	Vector2i& operator=(const Vector2i& a) { x = a.x; y = a.y; return *this; }
	Vector2i operator+(const Vector2i& a) const;
	Vector2i operator-(const Vector2i& a) const;
	Vector2i operator*(const Vector2i& a) const;
	Vector2i operator/(const Vector2i& a) const;
	Vector2i& operator+=(const Vector2i& a);
	Vector2i& operator-=(const Vector2i& a);
	Vector2i& operator*=(const Vector2i& a);
	Vector2i& operator/=(const Vector2i& a);

	float length() const;
	float lengthSquared() const;
	float dot() const;
	void normalize();
	Vector2i normalizeVector2() const;

	float distance(const Vector2i& b) const;

	bool operator==(const Vector2i& vector) const { return x == vector.x && y == vector.y; }
	bool operator!=(const Vector2i& vector) const { return x != vector.x || y != vector.y; }
};

