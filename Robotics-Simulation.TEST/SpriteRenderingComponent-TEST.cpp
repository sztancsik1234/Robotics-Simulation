#include "pch.h"  
#include "graphics/SpriteRendererComponent.h"  
#include "MockGameObject.h"
#include "MockLogger.h"
#include "MockRenderer.h"

TEST(SpriteRenderingComponentTest, ConstructorInitializesCorrectly) {
    // Arrange
	MockLogger logger;
	MockGameObject gameObject(logger);
	MockRenderer renderer;
	const char* texturePath = "Test texture.jpg";

    // Act
    SpriteRenderComponent spriteComponent(&gameObject, renderer, logger, texturePath);
    
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
	gameObject.EmplaceComponent<SpriteRenderComponent>(renderer, logger, texturePath);
    
    // Assert
	EXPECT_TRUE(renderer.loadTextureCalled);
	EXPECT_EQ(gameObject.GetComponent<SpriteRenderComponent>()->GetTextureId(), renderer.lastTextureId);
}

TEST(SpriteRenderingComponentTest, OnAddHandlesInvalidTexturePath) {
    // Arrange
	MockLogger logger;
	MockGameObject gameObject(logger);
	MockRenderer renderer;
	const char* invalidTexturePath = "Invalid texture.jpg";
    
    // Act
	auto spriteRenderer = gameObject.EmplaceComponent<SpriteRenderComponent>(renderer, logger, invalidTexturePath);
        
    // Assert
	EXPECT_TRUE(renderer.loadTextureCalled);
	EXPECT_EQ(spriteRenderer->GetTextureId(), MockRenderer::DummyDefaultTextureId);
	EXPECT_EQ(renderer.lastTextureId, MockRenderer::DummyDefaultTextureId);
}

TEST(SpriteRenderingComponentTest, UpdateCallsRenderer) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    const char* texturePath = "Test texture.jpg";
    auto spriteRenderer = gameObject.EmplaceComponent<SpriteRenderComponent>(renderer, logger, texturePath);
    
    // Act
	spriteRenderer->Update();
    
    // Assert
	EXPECT_TRUE(renderer.drawSpriteCalled);    
}

TEST(SpriteRenderingComponentTest, OnRemoveReleasesTexture) {
    // Arrange
    MockLogger logger;
    MockGameObject gameObject(logger);
    MockRenderer renderer;
    const char* texturePath = "Test texture.jpg";
    auto spriteRenderer = gameObject.EmplaceComponent<SpriteRenderComponent>(renderer, logger, texturePath);
    
    // Act
    gameObject.RemoveComponent(spriteRenderer);

    // Assert
    EXPECT_TRUE(renderer.unloadTextureCalled);
}

