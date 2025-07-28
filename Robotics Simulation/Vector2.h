#pragma once
#include "Exceptions.h"

struct Vector2
{
	float x;
	float y;

	Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
	explicit Vector2(const float arr[2]) : x(arr[0]), y(arr[1]) {}
	
	// Overload addition operator
	friend Vector2 operator+(Vector2 const& left, Vector2 const& right);
	// Overload subtraction operator
	friend Vector2 operator-(Vector2 const& left, Vector2 const& right);
	// Overload for scalar multiplication
	friend Vector2 operator*(Vector2 const& vec, float scalar);
	// Overload for scalar division operator
	friend Vector2 operator/(Vector2 const& vec, float scalar);
};