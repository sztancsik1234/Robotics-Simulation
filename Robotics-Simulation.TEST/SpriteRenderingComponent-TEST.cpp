#include "pch.h"  
#include "graphics/SpriteRendererComponent.h"  
#include "MockGameObject.h"
#include "MockLogger.h"
#include "MockRenderer.h"
#include "MockCameraRenderer.h"

TEST(SpriteRenderingComponentTest, ConstructorInitializesCorrectly) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    MockCameraRenderer camera;
    const char* texturePath = "Test texture.jpg";

    // Act
    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, texturePath);
    
    // Assert
	EXPECT_EQ(spriteComponent.GetTexturePath(), texturePath);
	EXPECT_EQ(spriteComponent.GetOwner(), &gameObject);
	EXPECT_EQ(spriteComponent.GetTextureId(), 0);
}

TEST(SpriteRenderingComponentTest, OnAddLoadsTexture) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    MockCameraRenderer camera;
    const char* texturePath = "Test texture.jpg";

    // Act
    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, texturePath);
    spriteComponent.OnAdd();

    // Assert
    EXPECT_TRUE(renderer.loadTextureCalled);
    EXPECT_EQ(spriteComponent.GetTextureId(), renderer.lastTextureId);
}

TEST(SpriteRenderingComponentTest, OnAddHandlesInvalidTexturePath) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    MockCameraRenderer camera;
    const char* invalidTexturePath = "Invalid texture.jpg";

    // Act
    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, invalidTexturePath);
    spriteComponent.OnAdd();

    // Assert
	EXPECT_TRUE(renderer.loadTextureCalled);
	EXPECT_EQ(spriteComponent.GetTextureId(), MockRenderer::DummyDefaultTextureId);
	EXPECT_EQ(renderer.lastTextureId, MockRenderer::DummyDefaultTextureId);
}

TEST(SpriteRenderingComponentTest, UpdateCallsRenderer) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    MockCameraRenderer camera;
    const char* texturePath = "Test texture.jpg";

    // Act
    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, texturePath);
    spriteComponent.OnAdd();
    spriteComponent.Update();

    // Assert
    EXPECT_TRUE(camera.drawSpriteCalled);
}

TEST(SpriteRenderingComponentTest, OnRemoveReleasesTexture) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    MockCameraRenderer camera;
    const char* texturePath = "Test texture.jpg";
    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, texturePath);
    spriteComponent.OnAdd();

    // Act
    spriteComponent.OnRemove();

    // Assert
    EXPECT_TRUE(renderer.unloadTextureCalled);
}

TEST(SpriteRenderingComponentTest, DrawsAtCorrectPosition) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    MockCameraRenderer camera;
    const char* texturePath = "Test texture.jpg";
    Vector2 testPosition(10, 15);
    gameObject.SetPosition(testPosition);

    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, texturePath);
    spriteComponent.OnAdd();

    // Act
    spriteComponent.Update();

    // Assert
    EXPECT_TRUE(camera.drawSpriteCalled);
    EXPECT_FLOAT_EQ(camera.lastWorldTransform.position.x, testPosition.x);
    EXPECT_FLOAT_EQ(camera.lastWorldTransform.position.y, testPosition.y);
}
