#pragma once
#include "core/Angle.h" 
#include "core/Vector2.h"
#include <memory>

enum class BodyType : char
{
	STATIC,
	KINEMATIC,
	DYNAMIC
};

enum class ShapeType : char
{
	NONE,
	CIRCLE,
	RECTANGLE
};


union ShapeData
{
	struct { float radius; } circle;
	struct { float width, height; } rectangle;
};

struct BodyDefinition {
	/* Body definition */
	Vector2 position = { 0, 0 };
	Radian rotation = 0.f;
	Vector2 anchor = { 0.5f, 0.5f };
	Vector2 initialVelocity = { 0, 0 };
	const char name[32] = "RigidBody";

	BodyType type = BodyType::STATIC;

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

	/* Polygon (shape) data */
	ShapeType shapeType = ShapeType::NONE;
	ShapeData shape {}; // zero-initialized; defaults to circle.radius = 0

	// TODO: separate into src file
	void SetShapeCircle(float radius)
	{
		shapeType = ShapeType::CIRCLE;
		shape = { .circle = { radius } };
	}
	void SetShapeRectangle(Vector2 size)
	{
		shapeType = ShapeType::RECTANGLE;
		shape = { .rectangle = { size.x, size.y } };
	}
};