#include <SFML/Graphics.hpp>
#include "graphics/SfmlRenderer.h"
#include "util/Exceptions.h"

/// <summary>
/// Constructs an SfmlRenderer object, initializing the window, clock, and deltaTime.
/// </summary>
SfmlRenderer::SfmlRenderer()
	: window(), clock(), deltaTime(0.0f)
{}

/// <summary>
/// Destructor for SfmlRenderer. Default implementation.
/// </summary>
SfmlRenderer::~SfmlRenderer() = default;

/// <summary>
/// Initializes the SFML window with a specific size and title.
/// Sets the frame rate limit to 60 FPS.
/// </summary>
void SfmlRenderer::Initialize()
{
	// Create a window with a specific size and title
	window.create(sf::VideoMode(sf::Vector2u(800, 600)), "SFML Renderer");
	window.setFramerateLimit(60); // Set the frame rate limit
}

/// <summary>
/// Clears the window with a black color.
/// </summary>
void SfmlRenderer::Clear(Color color = Color::Black)
{
	window.clear(ConvertColor(color)); // Clear the window with a black color
}

/// <summary>
/// Draws a line between two points.
/// </summary>
/// <param name="start">The starting point of the line.</param>
/// <param name="end">The endpoint of the line.</param>
void SfmlRenderer::DrawLine(Vector2 start, Vector2 end)
{
	throw NotImplementedException();
    //sf::Vertex line[] =
    //{
    //    sf::Vertex(sf::Vector2f(start.X, start.Y)),
    //    sf::Vertex(sf::Vector2f(end.X, end.Y))
    //};
    //window.draw(line, 2, sf::Lines);
}

/// <summary>
/// Draws a circle at the specified position with the given radius.
/// The circle is outlined in white and filled transparent.
/// </summary>
/// <param name="p">The center position of the circle.</param>
/// <param name="radius">The radius of the circle.</param>
void SfmlRenderer::DrawCircle(Vector2 p, float radius)  
{  
    sf::CircleShape circle(radius);
    // Set the origin to the center of the circle instead of the default top-left
	circle.setOrigin(sf::Vector2f({ radius, radius }));
    // Now we can position it directly at the point without manual adjustment
    circle.setPosition(sf::Vector2f(p.x, p.y));
    circle.setFillColor(sf::Color::Transparent);  
    circle.setOutlineColor(sf::Color::White);  
    circle.setOutlineThickness(1.0f);  
    window.draw(circle);  
}

/// <summary>
/// Draws a rectangle at the specified position with the given width and height.
/// The rectangle is outlined in white and filled transparent.
/// </summary>
/// <param name="p1">The top-left position of the rectangle.</param>
/// <param name="width">The width of the rectangle.</param>
/// <param name="height">The height of the rectangle.</param>
void SfmlRenderer::DrawRectangle(Vector2 p1, float width, float height)
{
	sf::RectangleShape rectangle(sf::Vector2f(width, height));
	rectangle.setPosition(sf::Vector2f(p1.x, p1.y));
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1.0f);
	window.draw(rectangle);
}

/// <summary>
/// Draws a rectangle defined by two points (top-left and bottom-right).
/// The rectangle is outlined in white and filled transparent.
/// </summary>
/// <param name="p1">The top-left position of the rectangle.</param>
/// <param name="p2">The bottom-right position of the rectangle.</param>
void SfmlRenderer::DrawRectangle(Vector2 p1, Vector2 p2)
{
	sf::RectangleShape rectangle(sf::Vector2f(p2.x - p1.x, p2.y - p1.y));
	rectangle.setPosition(sf::Vector2f(p1.x, p1.y));
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1.0f);
	window.draw(rectangle);
}

unsigned int SfmlRenderer::LoadTexture(const char* filePath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filePath)) {
		throw TextureLoadException(std::format("Failed to load texture from path: {}", filePath));
	}

	auto emplacedID = ++TextureKeyCounter;
	textures.emplace(emplacedID, std::move(texture));
	return emplacedID;
	
}

unsigned int SfmlRenderer::LoadTexture()
{
	sf::Image image(sf::Vector2u(200, 200), sf::Color::Magenta);
	sf::Texture texture(image);

	auto emplacedID = ++TextureKeyCounter;
	textures.emplace(emplacedID, std::move(texture));
	return emplacedID;
}

void SfmlRenderer::DrawSprite(Vector2 position, unsigned int textureId, Vector2 scale)
{
	try
	{
		sf::Sprite sprite(textures[textureId]);
		sprite.setPosition(position);
		sprite.setScale(scale);
		window.draw(sprite);
	}
	catch (const std::exception&)
	{
		throw TextureLoadException(std::format("[SfmlRenderer::DrawSprite] Sprite from Texture with id={} could not be created!", textureId));
	}

}

void SfmlRenderer::UnloadTexture(unsigned int textureId)
{
	textures.erase(textureId);
}


/// <summary>
/// Displays the contents of the window.
/// </summary>
void SfmlRenderer::DisplayFrame()
{
	window.display(); // Display the contents of the window
}

/// <summary>
/// Shuts down the renderer by closing the window.
/// </summary>
void SfmlRenderer::Shutdown()
{
	window.close(); // Close the window
}

sf::Color SfmlRenderer::ConvertColor(Color color) const
{
	switch (color)
	{
	case Color::Black: return sf::Color::Black;
	case Color::White: return sf::Color::White;
	case Color::Red: return sf::Color::Red;
	case Color::Green: return sf::Color::Green;
	case Color::Blue: return sf::Color::Blue;
	case Color::Yellow: return sf::Color(255, 255, 0);
	case Color::Magenta: return sf::Color(255, 0, 255);
	case Color::Cyan: return sf::Color(0, 255, 255);
	case Color::Transparent: return sf::Color(0, 0, 0, 0);
	default: return sf::Color(0, 0, 0); // Default to black if unknown
	}
}
