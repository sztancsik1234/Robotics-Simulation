#include "graphics/UiCameraRenderer.h"

void UiCameraRenderer::DrawCircle(Vector2 worldCenter, float worldRadius)
{
	Logger->Log("Not implemented!", LogLevel::WARNING);
}

void UiCameraRenderer::DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize)
{
	Logger->Log("Not implemented!", LogLevel::WARNING);
}

void UiCameraRenderer::DrawRectangle(Vector2 worldP1, Vector2 worldP2)
{
	Logger->Log("Not implemented!", LogLevel::WARNING);
}

void UiCameraRenderer::DrawSprite(const Transform & worldTransform, TextureId textureId, const Vector2 spriteAnchor)
{
#ifdef _DEBUG
    if (Logger && textureId == 0)
    {
        Logger->Log("[CameraRenderer] TextureId is 0, sprite will not be drawn!", LogLevel::WARNING);
        return;
    }
#endif
    const Transform screen = Viewport.ToScreenSpace(worldTransform);
    Renderer.DrawSprite(screen, textureId, spriteAnchor);   // Size is y=0 for some reason
}
