#include "pch.h"  
#include "graphics/SpriteRendererComponent.h"  
#include "MockGameObject.h"
#include "MockLogger.h"
#include "MockRenderer.h"

TEST(SpriteRenderingComponentTest, ConstructorInitializesCorrectly) {
    // Arrange
	MockLogger logger;
	GameObject gameObject(logger);
	MockRenderer renderer;
	Camera camera(renderer, logger, { 0, 0 }, { 800, 600 }, { 800, 600 });
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
    Camera camera(renderer, logger, { 0, 0 }, { 800, 600 }, { 800, 600 });

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
    Camera camera(renderer, logger, { 0, 0 }, { 800, 600 }, { 800, 600 });

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
    Camera camera(renderer, logger, { 0, 0 }, { 800, 600 }, { 800, 600 });

    const char* texturePath = "Test texture.jpg";
    
    // Act
    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, texturePath);
    spriteComponent.OnAdd();
    spriteComponent.Update();
    
    // Assert
	EXPECT_TRUE(renderer.drawSpriteCalled);    
}

TEST(SpriteRenderingComponentTest, OnRemoveReleasesTexture) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    Camera camera(renderer, logger, { 0, 0 }, { 800, 600 }, { 800, 600 });

    const char* texturePath = "Test texture.jpg";
    SpriteRenderComponent spriteComponent(&gameObject, camera, renderer, logger, texturePath);
    spriteComponent.OnAdd();
    
    // Act
	spriteComponent.OnRemove();

    // Assert
    EXPECT_TRUE(renderer.unloadTextureCalled);
}

