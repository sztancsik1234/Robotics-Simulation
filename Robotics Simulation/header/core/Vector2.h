#pragma once
#include "util/Exceptions.h"
#include <concepts>

// Concept to check if a type has accessible x and y members
template<typename T>
concept HasXYMembers = requires(T t) {
	t.x;  // Must have an x member
	t.y;  // Must have an y member
	// Check that x and y are assignable from float
	{ t.x = float{} };
	{ t.y = float{} };
};

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

	// Template for converting to any class that has x and y properties
	template<HasXYMembers T>
	constexpr T convertTo() const
	{
		T result{};
		result.x = static_cast<decltype(result.x)>(this->x);
		result.y = static_cast<decltype(result.y)>(this->y);
		return result;
	}

	// Conversion operator template for implicit conversions
	template<HasXYMembers T>
	constexpr operator T() const
	{
		return convertTo<T>();
	}
};