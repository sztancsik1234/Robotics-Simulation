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
	const char* texturePath = "Test texture.jpg";

    // Act
    SpriteRenderComponent spriteComponent(&gameObject, renderer, logger, texturePath, {200, 200});
    
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
	const char* texturePath = "Test texture.jpg";

    // Act
	SpriteRenderComponent spriteComponent(&gameObject, renderer, logger, texturePath, { 200, 200 });
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
	const char* invalidTexturePath = "Invalid texture.jpg";
    
    // Act
    SpriteRenderComponent spriteComponent(&gameObject, renderer, logger, invalidTexturePath, { 200, 200 });
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
    const char* texturePath = "Test texture.jpg";
    
    // Act
    SpriteRenderComponent spriteComponent(&gameObject, renderer, logger, texturePath, { 200, 200 });
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
    const char* texturePath = "Test texture.jpg";
    SpriteRenderComponent spriteComponent(&gameObject, renderer, logger, texturePath, { 200, 200 });
    spriteComponent.OnAdd();
    
    // Act
	spriteComponent.OnRemove();

    // Assert
    EXPECT_TRUE(renderer.unloadTextureCalled);
}

