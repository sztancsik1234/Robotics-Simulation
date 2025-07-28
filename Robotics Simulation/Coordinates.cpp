#include "Vector2.h"

// Overload addition operator
Vector2 operator+(Vector2 const& left, Vector2 const& right) {
	Vector2 result = left;
	result.x += right.x;
	result.y += right.y;
	return result;
}

// Overload subtraction operator
Vector2 operator-(Vector2 const& left, Vector2 const& right) {
	Vector2 result = left;
	result.x -= right.x;
	result.y -= right.y;
	return result;
}

// Overload for scalar multiplication
Vector2 operator*(Vector2 const& vec, float scalar) {
	Vector2 result = vec;
	result.x *= scalar;
	result.y *= scalar;
	return result;
}

// Overload for scalar division operator
Vector2 operator/(Vector2 const& vec, float scalar) {
	if (scalar == 0.0f) {
		throw DivisionByZeroException();
	}
	Vector2 result = vec;
	result.x /= scalar;
	result.y /= scalar;
	return result;
}
