#pragma once
#include "RenderComponent.h"

class SpriteRenderComponent final : public RenderComponent
{
public:
	// can not pass ISpriteRenderer, because rendercomponent needs IDrawableRenderer and it can not be casted
	SpriteRenderComponent(GameObject* owner, ISpriteRenderer& renderer, ILogger& logger, const char* texturePath):
		RenderComponent(owner, logger),
		Renderer(renderer),
		TexturePath(texturePath)
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;

private:
	ISpriteRenderer& Renderer;
	unsigned int TextureId = 0;
	const char* TexturePath;
};