#pragma once
#include "RenderComponent.h"

class SpriteRenderComponent final : public RenderComponent
{
public:
	// can not pass ISpriteRenderer, because rendercomponent needs IDrawableRenderer and it can not be casted
	SpriteRenderComponent(GameObject* owner, ISpriteRenderer& renderer, ILogger& logger, const char* texturePath, Vector2 size, Vector2 ancour = {0.f,0.f}) :
		RenderComponent(owner, logger),
		Renderer(renderer),
		TexturePath(texturePath),
		Size(size),
		SpriteAncor(ancour)
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;

	const char* GetTexturePath() const { return TexturePath; }
	const unsigned int GetTextureId() const { return TextureId; }
	void SetSize(const Vector2& size) { Size = size; }
	void SetSize(const Vector2&& size) { Size = size; }

private:
	ISpriteRenderer& Renderer;
	unsigned int TextureId = 0;
	const char* TexturePath;
	Vector2 Size;
	Vector2 SpriteAncor; // default top-left corner
};