#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "graphics/IRenderer.h"
#include "core/Color.inl"


class SfmlRenderer : public IRenderer
{
public:
	SfmlRenderer();
	~SfmlRenderer() override;
	void Initialize() override;
	void Clear(Color color) override;
	void DrawLine(Vector2 x, Vector2 y) override;
	void DrawCircle(Vector2 p, float radius) override;
	void DrawRectangle(Vector2 p1, float width, float height) override;
	void DrawRectangle(Vector2 p1, Vector2 p2) override;
	unsigned int LoadTexture(const char* filePath) override;
	unsigned int LoadTexture() override; // Fallback if path could not be loaded
										// not overriding? Why?
	void DrawSprite(Vector2 position, unsigned int textureId, Vector2 size, const Vector2 SpriteAncor = { 0.f,0.f });
	void UnloadTexture(unsigned int textureId) override;
	void DisplayFrame() override;
	void Shutdown() override;

private:
	sf::RenderWindow window;
	sf::Clock clock;
	float deltaTime = 0.0f;

	unsigned int TextureKeyCounter = 0;
	std::unordered_map<unsigned int, sf::Texture> textures;	// conciderations have been made for the type of container used here. In the end, unordered_map was chosen for its simplicity and overhead. 
															// If performance becomes an issue, raw memory addresses could be used as id, but I pity the guy, who would need to debug that.

	// color to sf::color
	sf::Color ConvertColor(Color color) const;

	friend class SfmlInputService;
};

