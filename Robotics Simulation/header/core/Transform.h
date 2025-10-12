#pragma once
#include "core/Angle.h"
#include "core/Vector2.h"

struct Transform
{
	Radian Rotation = Radian(0.0f);
	Vector2 Position = Vector2(0.0f, 0.0f);
};