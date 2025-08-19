#pragma once
#include "IRenderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "pch.h"


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
	void DrawSprite(Vector2 p1, float width, float height, const char* texturePath) override;
	void DisplayFrame() override;
	void Shutdown() override;

private:
	sf::RenderWindow window;
	sf::Clock clock;
	float deltaTime = 0.0f;

	// sf::color to color
	sf::Color ConvertColor(Color color) const;

	friend class SfmlInputService;
};

