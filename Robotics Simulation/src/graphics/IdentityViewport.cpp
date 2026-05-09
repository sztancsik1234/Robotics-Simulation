#include "graphics/IdentityViewport.h"

Vector2 IdentityViewport::GetViewCenter() const
{
    return Vector2();
}

Vector2 IdentityViewport::GetViewSize() const
{
    return Vector2();
}

Vector2 IdentityViewport::GetScreenResolution() const
{
    return Vector2();
}

Vector2 IdentityViewport::PixelToWorldPos(Vector2 pixelCoords) const
{
    return pixelCoords;
}

Vector2 IdentityViewport::WorldToPixelPos(Vector2 worldPos) const
{
    return worldPos;
}

Transform IdentityViewport::ToScreenSpace(const Transform& world) const
{
    return world;
}
