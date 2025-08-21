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

void MockRenderer::DrawSprite(Vector2 position, float width, float height, const char* texturePath) {
    throw NotImplementedException();
}

// MockLogger implementation
void MockLogger::Log(const std::string& message, LogLevel level) {
    lastMessage = message;
    lastLevel = level;
    logCalled = true;
}