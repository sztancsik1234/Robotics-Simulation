#pragma once
#include "core/Vector2.h"
#include "core/Angle.h" 

struct BodyDefinition {
	bool Wheel = false;
	float anglularDamping = 0.f;
	float linearDamping = 0.f;
	Vector2 position = { 0, 0 };
	Angle rotation = 0.f;
	Vector2 initialVelocity = { 0, 0 };
	bool isRotationFixed = false;
	bool isBullet = false;

};