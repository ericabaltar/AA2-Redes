#pragma once
#include <math.h>
struct Vector2
{
	float x, y;

	Vector2() : x(0), y(0) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	const inline float SqrMagnitude() const { return x * x + y * y; }
	const inline float Magnitude() const { return sqrtf(SqrMagnitude()); }

	const Vector2 operator+ (Vector2 other) const { return Vector2(other.x + x, other.y + y); }
	const Vector2 operator- () const { return Vector2(-x, -y); }
	const Vector2 operator- (Vector2 other) const { return (*this) + (-other); }
};

