#include "core/GameObject.h"
#include "graphics/SpriteRendererComponent.h"

void SpriteRenderComponent::OnAdd()
{
	try
	{
		TextureId = Renderer.LoadTexture(TexturePath);
	}
	catch (const TextureLoadException&)
	{
		//Logger.Log(std::format("[SpriteComponent] {}\n\tDefaulting do default texture" + std::string(e.what())), LogLevel::WARNING);
		TextureId = Renderer.LoadTexture();
	}

}

void SpriteRenderComponent::Update()
{
	// in debug check textureid
	if (TextureId == 0)
	{
		Logger.Log("[SpriteRenderComponent] TextureId is 0, sprite will not be drawn!", LogLevel::WARNING);
		return;
	}
  
	Renderer.DrawSprite(GetOwner()->GetPosition(), TextureId, Size, SpriteAncor);
}

void SpriteRenderComponent::OnRemove()
{
	Renderer.UnloadTexture(TextureId);
	Logger.Log("[SpriteRenderComponent] Texture unloaded.", LogLevel::INFO);
}
