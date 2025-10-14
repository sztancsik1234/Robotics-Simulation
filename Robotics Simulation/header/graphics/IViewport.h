#pragma once
#include "core/Transform.h"

struct IViewport
{
    virtual ~IViewport() = default;

    virtual Vector2 GetViewCenter() const = 0;
    virtual Vector2 GetViewSize() const = 0;
    virtual Vector2 GetScreenResolution() const = 0;

    // Symmetric conversion API
    virtual Vector2 PixelToWorldPos(Vector2 pixelCoords, bool snapToPixel = true) const = 0;
    virtual Vector2 WorldToPixelPos(Vector2 worldPos, bool snapToPixel = true) const = 0;
    virtual Transform ToScreenSpace(const Transform& world, bool snapToPixel = true) const = 0;
};