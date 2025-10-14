#include "pch.h"
#include "graphics/CircleRenderer.h"
#include "core/Vector2.h"
#include "graphics/IRenderer.h"
#include "util/ILogger.h"
#include "core/GameObject.h"
#include "util/Exceptions.h"
#include "MockRenderer.h"
#include "MockLogger.h"
#include "MockViewport.h"



// MockViewport implementation
Vector2 MockViewport::GetViewCenter() const { return viewCenter; }
Vector2 MockViewport::GetViewSize() const { return viewSize; }
Vector2 MockViewport::GetScreenResolution() const { return screenSize; }

Vector2 MockViewport::PixelToWorldPos(Vector2 px, bool snapToPixel) const
{
    if (snapToPixel) { px.x = std::round(px.x); px.y = std::round(px.y); }
    // Simple symmetrical mapping for tests (no Y flip):
    const Vector2 topLeft{ viewCenter.x - viewSize.x * 0.5f, viewCenter.y - viewSize.y * 0.5f };
    return {
        topLeft.x + (px.x * (viewSize.x / screenSize.x)),
        topLeft.y + (px.y * (viewSize.y / screenSize.y))
    };
}

Vector2 MockViewport::WorldToPixelPos(Vector2 wp, bool snapToPixel) const
{
    const Vector2 topLeft{ viewCenter.x - viewSize.x * 0.5f, viewCenter.y - viewSize.y * 0.5f };
    Vector2 px{
        (wp.x - topLeft.x) * (screenSize.x / viewSize.x),
        (wp.y - topLeft.y) * (screenSize.y / viewSize.y)
    };
    if (snapToPixel) { px.x = std::round(px.x); px.y = std::round(px.y); }
    return px;
}

Transform MockViewport::ToScreenSpace(const Transform& world, bool snapToPixel) const
{
    Transform t{};
    t.position = WorldToPixelPos(world.position, snapToPixel);
    t.size = {
        std::max(1.f, std::round(world.size.x * (screenSize.x / viewSize.x))),
        std::max(1.f, std::round(world.size.y * (screenSize.y / viewSize.y)))
    };
    t.rotation = world.rotation;
    return t;
}

// MockRenderer implementation
void MockRenderer::DrawCircle(Vector2 position, float radius) {
    drawCircleCalled = true;
    lastPosition = position;
    lastRadius = radius;
}

void MockRenderer::DrawLine(Vector2 start, Vector2 end) {
    throw NotImplementedException();
}

void MockRenderer::DrawRectangle(Vector2 topLeft, float width, float height) {
    throw NotImplementedException();
}

void MockRenderer::DrawRectangle(Vector2 p1, Vector2 p2) {
    throw NotImplementedException();
}

void MockRenderer::DrawSprite(const Transform& transform, TextureId textureId, const Vector2 SpriteAnchor)
{
	drawSpriteCalled = true;
	lastPosition = transform.position;
	lastTextureId = textureId;
	lastSize = transform.size;
	lastAncor = SpriteAnchor;
}

unsigned int MockRenderer::LoadTexture(const std::string filePath)
{
    if (std::string(filePath) != "Test texture.jpg") {
        throw TextureLoadException("Failed to load texture from path: " + std::string(filePath));
	}
	loadTextureCalled = true;
	lastTextureId = DummyTextureId;
	return DummyTextureId; // Return a dummy texture ID
}

unsigned int MockRenderer::LoadTexture()
{
	loadTextureCalled = true;
	lastTextureId = DummyDefaultTextureId;
    return MockRenderer::DummyDefaultTextureId;
}

void MockRenderer::UnloadTexture(unsigned int textureId)
{
	unloadTextureCalled = true;
}

// MockLogger implementation
void MockLogger::Log(const std::string message, LogLevel level) {
    lastMessage = message;
    lastLevel = level;
    logCalled = true;
}