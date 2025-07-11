#include "pch.h"
#include "SfmlRenderer.h"
#include <SFML/Graphics.hpp>



SfmlRenderer::SfmlRenderer()
	: window(), clock(), deltaTime(0.0f)
{}


SfmlRenderer::~SfmlRenderer() = default;

void SfmlRenderer::Initialize()
{
	// Create a window with a specific size and title
	window.create(sf::VideoMode(sf::Vector2u(800, 600)), "SFML Renderer");
	window.setFramerateLimit(60); // Set the frame rate limit
}

void SfmlRenderer::Clear()
{
	window.clear(sf::Color::Black); // Clear the window with a black color
}

void SfmlRenderer::DrawLine(Vector2 p)
{
	throw NotImplementedException();
}

void SfmlRenderer::DrawCircle(Vector2 p, float radius)  
{  
	sf::CircleShape circle(radius);  
	circle.setPosition(sf::Vector2f(p.x - radius, p.y - radius));  
	circle.setFillColor(sf::Color::Transparent);  
	circle.setOutlineColor(sf::Color::White);  
	circle.setOutlineThickness(1.0f);  
	window.draw(circle);  
}

void SfmlRenderer::DrawRectangle(Vector2 p1, float width, float height)
{
	sf::RectangleShape rectangle(sf::Vector2f(width, height));
	rectangle.setPosition(sf::Vector2f(p1.x, p1.y));
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1.0f);
	window.draw(rectangle);
}

void SfmlRenderer::DrawRectangle(Vector2 p1, Vector2 p2)
{
	sf::RectangleShape rectangle(sf::Vector2f(p2.x - p1.x, p2.y - p1.y));
	rectangle.setPosition(sf::Vector2f(p1.x, p1.y));
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1.0f);
	window.draw(rectangle);
}

void SfmlRenderer::DrawSprite(Vector2 p1, float width, float height, const char* texturePath)
{
	throw NotImplementedException();
}

void SfmlRenderer::Render()
{
	window.display(); // Display the contents of the window
}

void SfmlRenderer::Shutdown()
{
	throw NotImplementedException();
}

