#pragma once
#include <string>
#include "core/Vector2.h"

// -----------------------------
// SpriteRenderComponent
// Constructor: SpriteRenderComponent(GameObject*, ISpriteRenderer&, ILogger&, const std::string& texturePath, Vector2 size, Vector2 anchor)
// Services: ISpriteRenderer&, ILogger&
// Data: texturePath, size, anchor
struct SpriteRenderComponentDTO
{
    std::string texturePath;
    Vector2     size;
    Vector2     anchor {0.f, 0.f};
};

// -----------------------------
// CircleRenderer
// Constructor: CircleRenderer(GameObject*, IPrimitiveRenderer&, ILogger&)
// Services: IPrimitiveRenderer&, ILogger&
// Data: (none currently)
// If later you add visual parameters (radius, color, thickness, etc.), put them here.
struct CircleRendererDTO
{
    // Intentionally empty: no construction-time data beyond services.
};

// -----------------------------
// MouseFollowerComponent
// Constructor: MouseFollowerComponent(GameObject*, IInputService&)
// Services: IInputService&
// Data: (none)
// Add configurable behaviour (e.g., smoothing factor) here later if needed.
struct MouseFollowerComponentDTO
{
    // Intentionally empty: no data-only fields required.
};
