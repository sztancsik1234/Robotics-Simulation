#pragma once

#include "graphics/IViewport.h"
#include "core/Vector2.h"
#include "core/Transform.h"

class MockViewport : public IViewport
{
public:
    // IViewport interface implementation
    Vector2 GetViewCenter() const override;
    Vector2 GetViewSize() const override;
    Vector2 GetScreenResolution() const override;
    
    Vector2 PixelToWorldPos(Vector2 px) const override;
    Vector2 WorldToPixelPos(Vector2 wp) const override;
    Transform ToScreenSpace(const Transform& world) const override;

    // Mock data members - can be set by tests
    Vector2 viewCenter{ 0.0f, 0.0f };
    Vector2 viewSize{ 800.0f, 600.0f };
    Vector2 screenSize{ 800.0f, 600.0f };
};
