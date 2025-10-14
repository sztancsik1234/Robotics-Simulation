#pragma once
#include "graphics/IViewport.h"

// MockViewport: A mock implementation of IViewport for testing purposes. For this class, world space is screen space.
class MockViewport final : public IViewport
{
public:
    Vector2 viewCenter{ 0, 0 };
    Vector2 viewSize{ 1000, 1000 };
    Vector2 screenSize{ 1000, 1000 };

    Vector2 GetViewCenter() const override;
    Vector2 GetViewSize() const override;
    Vector2 GetScreenResolution() const override;

    Vector2 PixelToWorldPos(Vector2 px, bool snapToPixel = true) const override;
    Vector2 WorldToPixelPos(Vector2 wp, bool snapToPixel = true) const override;
    Transform ToScreenSpace(const Transform& world, bool snapToPixel = true) const override;
};

