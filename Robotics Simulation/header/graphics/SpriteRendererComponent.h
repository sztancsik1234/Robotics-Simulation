#pragma once
#include "RenderComponent.h"
#include "core/ComponentDTOs.h"

class SpriteRenderComponent final : public RenderComponent
{
public:
	SpriteRenderComponent(GameObject* owner,
		ISpriteRenderer& renderer,
		ILogger& logger,
		const std::string& texturePath,
		Vector2 anchor = {0.f,0.f}) :
		RenderComponent(owner, logger),
		Renderer(renderer),
		TexturePath(texturePath),
		SpriteAnchor(anchor)
	{}

	SpriteRenderComponent(GameObject* owner,
		ISpriteRenderer& renderer,
		ILogger& logger,
		const SpriteRenderComponentDTO& DTO) :
		RenderComponent(owner, logger),
		Renderer(renderer),
		TexturePath(DTO.texturePath),
		SpriteAnchor(DTO.anchor)
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;

	std::string GetTexturePath() const { return TexturePath; }
	TextureId GetTextureId() const { return TextureId; }

private:
	ISpriteRenderer& Renderer;
	TextureId TextureId = 0;
	const std::string TexturePath;
	Vector2 SpriteAnchor; // default top-left corner
};