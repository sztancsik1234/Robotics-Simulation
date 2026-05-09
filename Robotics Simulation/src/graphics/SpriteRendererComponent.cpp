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
#ifdef _DEBUG
	if (TextureId == 0)
	{
		Logger.Log("[SpriteRenderComponent] TextureId is 0, sprite will not be drawn!", LogLevel::WARNING);
		return;
	}
#endif // _DEBUG
	auto transform = GetOwner()->GetTransform();
	auto anchor = GetOwner()->GetAnchor();

	camera.DrawSprite(transform, TextureId, anchor);
}

void SpriteRenderComponent::OnRemove()
{
	Renderer.UnloadTexture(TextureId);
	Logger.Log("[SpriteRenderComponent] Texture unloaded.", LogLevel::INFO);
}

std::string SpriteRenderComponent::ToString() const
{
	return std::format("<SpriteRenderComponent> image={}", TexturePath);
}
