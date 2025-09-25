#pragma once
#include "RenderComponent.h"

class SpriteRenderComponent final : public RenderComponent
{
public:
	SpriteRenderComponent(GameObject* owner, 
		ISpriteRenderer& renderer, 
		ILogger& logger, 
		const std::string& texturePath, 
		Vector2 size, 
		Vector2 anchor = {0.f,0.f}) :
		RenderComponent(owner, logger),
		Renderer(renderer),
		TexturePath(texturePath),
		Size(size),
		SpriteAnchor(anchor)
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;

	std::string GetTexturePath() const { return TexturePath; }
	TextureId GetTextureId() const { return TextureId; }
	void SetSize(const Vector2& size) { Size = size; }
	void SetSize(Vector2 size) { Size = size; }

private:
	ISpriteRenderer& Renderer;
	TextureId TextureId = 0;
	const std::string TexturePath;
	Vector2 Size;
	Vector2 SpriteAnchor; // default top-left corner
};