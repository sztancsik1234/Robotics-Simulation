#include "pch.h"
#include "graphics/CircleRenderer.h"
#include "core/Vector2.h"
#include "graphics/IRenderer.h"
#include "util/ILogger.h"
#include "core/GameObject.h"
#include "util/Exceptions.h"
#include "MockRenderer.h"
#include "MockLogger.h"


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

void MockRenderer::DrawSprite(Vector2 position, unsigned int textureId, Vector2 size, const Vector2 SpriteAnchor)
{
	drawSpriteCalled = true;
	lastPosition = position;
	lastTextureId = textureId;
	lastSize = size;
	lastAncor = SpriteAnchor;
}

unsigned int MockRenderer::LoadTexture(const char* filePath)
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