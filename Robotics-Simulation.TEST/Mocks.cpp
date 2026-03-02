#include "pch.h"
#include "graphics/CircleRendererComponent.h"
#include "core/Vector2.h"
#include "graphics/IRenderer.h"
#include "util/ILogger.h"
#include "core/GameObject.h"
#include "util/Exceptions.h"
#include "MockRenderer.h"
#include "MockLogger.h"
#include "MockViewport.h"
#include "MockPhysicsEngine.h"
#include "MockCameraRenderer.h"

/* MockCameraRenderer implementation */
void MockCameraRenderer::DrawCircle(Vector2 worldCenter, float worldRadius)
{
    drawCircleCalled = true;
    lastWorldCenter = worldCenter;
    lastWorldRadius = worldRadius;
}

void MockCameraRenderer::DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize)
{
    drawRectangleTopLeftCalled = true;
    lastWorldTopLeft = worldTopLeft;
    lastWorldSize = worldSize;
}

void MockCameraRenderer::DrawRectangle(Vector2 worldP1, Vector2 worldP2)
{
    drawRectangleCalled = true;
    lastWorldP1 = worldP1;
    lastWorldP2 = worldP2;
}

void MockCameraRenderer::DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor)
{
    drawSpriteCalled = true;
    lastWorldTransform = worldTransform;
    lastTextureId = textureId;
    lastSpriteAnchor = spriteAnchor;
}

/* MockViewport implementation */
Vector2 MockViewport::GetViewCenter() const { return viewCenter; }
Vector2 MockViewport::GetViewSize() const { return viewSize; }
Vector2 MockViewport::GetScreenResolution() const { return screenSize; }

Vector2 MockViewport::PixelToWorldPos(Vector2 px) const
{
    return px;
}

Vector2 MockViewport::WorldToPixelPos(Vector2 wp) const
{
    return wp;
}

Transform MockViewport::ToScreenSpace(const Transform& world) const
{
    return world;
}

/* MockRenderer implementation */
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

/* MockLogger implementation */
void MockLogger::Log(const std::string message, LogLevel level) {
    lastMessage = message;
    lastLevel = level;
    logCalled = true;
}

/* MockPhysicsEngine implementation */
void MockPhysicsEngine::Initialize()
{
    initializeCalled = true;
    isInitialized_ = true;
}

void MockPhysicsEngine::Shutdown()
{
    shutdownCalled = true;
    isInitialized_ = false;
    bodies_.clear();
    nextId_ = 1;
}

void MockPhysicsEngine::simulateStep(float deltaSeconds)
{
    simulateStepCalled = true;
    lastDeltaSeconds = deltaSeconds;

    for (auto& [id, body] : bodies_)
    {
        (void)id;
        if (body.type == BodyType::STATIC)
            continue;

        // Simple Euler integration; mass = 1, no damping for simplicity.
        body.position.x += body.velocity.x * deltaSeconds;
        body.position.y += body.velocity.y * deltaSeconds;
        // Rotation integration omitted unless needed by tests.
    }
}

bool MockPhysicsEngine::IsInitialized() const
{
    return isInitialized_;
}

BodyId MockPhysicsEngine::createBody(const BodyDefinition& bodyDef)
{
    createBodyCalled = true;

    const BodyId id = nextId_++;
    BodyData data{};
    data.position = bodyDef.position;
    data.rotation = bodyDef.rotation;
    data.velocity = bodyDef.initialVelocity;
    data.type = bodyDef.type;

    bodies_.try_emplace(id, data);
    lastBodyId = id;
    return id;
}

void MockPhysicsEngine::destroyBody(BodyId id)
{
    destroyBodyCalled = true;
    lastBodyId = id;
    bodies_.erase(id);
}

Vector2 MockPhysicsEngine::getBodyPosition(BodyId id) const
{
    if (auto it = bodies_.find(id); it != bodies_.end())
    {
        return it->second.position;
    }
    return Vector2{ 0.f, 0.f };
}

Radian MockPhysicsEngine::getBodyRotation(BodyId id) const
{
    if (auto it = bodies_.find(id); it != bodies_.end())
    {
        return it->second.rotation;
    }
    return Radian(0.f);
}

void MockPhysicsEngine::applyForceToBody(BodyId id, const Vector2& force, float timeWindow)
{
    applyForceCalled = true;
    lastBodyId = id;
    lastAppliedForce = force;
    lastTimeWindow = timeWindow;

    auto it = bodies_.find(id);
    if (it == bodies_.end())
        return;

    // Assume mass = 1; dv = a * dt = F/m * dt
    it->second.velocity.x += force.x * timeWindow;
    it->second.velocity.y += force.y * timeWindow;
}

void MockPhysicsEngine::setSpeed(BodyId id, const Vector2& impulse)
{
    setSpeedCalled = true;
    lastBodyId = id;
    lastSetSpeed = impulse;

    auto it = bodies_.find(id);
    if (it == bodies_.end())
        return;

    it->second.velocity = impulse;
}