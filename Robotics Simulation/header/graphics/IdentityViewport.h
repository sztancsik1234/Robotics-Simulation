#pragma once
#include "IViewport.h"
class IdentityViewport : public IViewport
{
public:
	IdentityViewport() : IViewport() {}
	// Inherited via IViewport
	Vector2 GetViewCenter() const override;
	Vector2 GetViewSize() const override;
	Vector2 GetScreenResolution() const override;
	Vector2 PixelToWorldPos(Vector2 pixelCoords) const override;
	Vector2 WorldToPixelPos(Vector2 worldPos) const override;
	Transform ToScreenSpace(const Transform& world) const override;
};

