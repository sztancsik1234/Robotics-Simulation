#pragma once
#include "core/Vector2.h"
#include "core/Angle.h" 
#include <memory>

enum class BodyType : char
{
	Static,
	Kinematic,
	Dynamic
};

struct Shape
{
	virtual ~Shape() = default;
};

struct Circle : Shape
{
	float radius;
};

struct Rectangle : Shape
{
	float width, height;
	Rectangle(float width, float height) : width(width), height(height) {}
	Rectangle(Vector2 size) : width(size.x), height(size.y) {}
};

struct BodyDefinition {
	/* Body definition */
	Vector2 position = { 0, 0 };
	Radian rotation = 0.f;
	Vector2 initialVelocity = { 0, 0 };
	const char name[32] = "RigidBody";

	BodyType type = BodyType::Static;

	float anglularDamping = 0.f;
	float linearDamping = 0.f;

	bool Wheel = false;
	bool isRotationFixed = false;
	bool isBullet = false;
	float sleepTreshold = 0.05f;

	/* Shape definition */ 
	float density = 0;			// Mass per unit area in kg/m^2
	float friction = 0;			// Coefficient of friction, typically between 0 and 1
	float restitution = 1;		// Bounciness, Coefficent of speed between before and after the collision, typically between 0 and 1
	float rollingFriction = 0;	// Coefficient of rolling friction, typically between 0 and 1
	float tangentSpeed = 0;		// Speed at which the shape moves along its tangent, useful for simulating conveyor belts or similar effects

	/* Polygon definition */
	std::unique_ptr<Shape> shape = nullptr;
};