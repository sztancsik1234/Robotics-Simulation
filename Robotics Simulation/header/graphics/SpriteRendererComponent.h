#pragma once
#include "core/ComponentDTOs.h"
#include "core/GameObject.h"
#include "ICameraRenderer.h"
#include "IRenderer.h"
#include "RenderComponent.h"
#include "core/Vector2.h"
#include <string>
#include "util/ILogger.h"

class SpriteRenderComponent final : public RenderComponent
{
public:
	SpriteRenderComponent(GameObject* owner,
		ICameraRenderer& camera,
		ISpriteRenderer& renderer,
		ILogger& logger,
		const std::string& texturePath,
		Vector2 anchor = { 0.f,0.f }) :
		RenderComponent(owner, logger, camera),
		Renderer(renderer),
		TexturePath(texturePath)
	{}

	SpriteRenderComponent(GameObject* owner,
		ICameraRenderer& camera,
		ISpriteRenderer& renderer,
		ILogger& logger,
		const SpriteRenderComponentDTO& DTO) :
		RenderComponent(owner, logger, camera),
		Renderer(renderer),
		TexturePath(DTO.texturePath)
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;
	void Disable() override;
	void Enable() override;
	std::string ToString() const override;

	std::string GetTexturePath() const { return TexturePath; }
	TextureId GetTextureId() const { return TextureId; }

private:
	ISpriteRenderer& Renderer;
	TextureId TextureId = 0;
	const std::string TexturePath;
};