#pragma once
#include "core/Angle.h"
#include "core/Vector2.h"

struct Transform
{
	Vector2 position = Vector2(0.0f, 0.0f);
	Radian rotation = Radian(0.0f);
	Vector2 size = Vector2(0.f, 0.f);
};