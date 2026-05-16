#include <SFML/Graphics.hpp>
#include "graphics/SfmlRenderer.h"
#include "util/Exceptions.h"

/// <summary>
/// Constructs an SfmlRenderer object, initializing the window, clock, and deltaTime.
/// </summary>
SfmlRenderer::SfmlRenderer(ILogger& logger)
	: Logger(logger), window(), clock(), deltaTime(0.0f)
{}

/// <summary>
/// Destructor for SfmlRenderer. Default implementation.
/// </summary>
SfmlRenderer::~SfmlRenderer() = default;

void SfmlRenderer::Initialize(Vector2 screenSize)
{
	// Create a window with a specific size and title
	window.create(sf::VideoMode(sf::Vector2u(screenSize.x, screenSize.y)), "SFML Renderer", sf::Style::Close);
	window.setFramerateLimit(60); // Set the frame rate limit
}

bool SfmlRenderer::IsInitialized() const
{
	if (window.isOpen())
		return true;
	return false;
}

sf::RenderWindow& SfmlRenderer::GetWindow()
{
	return window;
}

void SfmlRenderer::Clear(Color color = Color::Black)
{
	window.clear(ConvertColor(color)); // Clear the window with a black color
}

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

void SfmlRenderer::DrawCircle(Vector2 p, float radius)  
{  
    sf::CircleShape circle(radius);
    // Set the origin to the center of the circle instead of the default top-left
	circle.setOrigin(sf::Vector2f({ radius, radius }));
    // Now we can position it directly at the point without manual adjustment
    circle.setPosition(sf::Vector2f(p.x, p.y));
    circle.setFillColor(sf::Color::Red);  
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

TextureId SfmlRenderer::LoadTexture(const std::string filePath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filePath)) {
		throw TextureLoadException(std::format("Failed to load texture from path: {}", filePath));
	}

	TextureId emplacedID = (TextureId)++TextureKeyCounter;
	textures.emplace(emplacedID, std::move(texture));
	return emplacedID;
}

TextureId SfmlRenderer::LoadTexture()
{
	sf::Image image(sf::Vector2u(200, 200), sf::Color::Magenta);
	sf::Texture texture(image);

	auto emplacedID = (TextureId)++TextureKeyCounter;
	textures.emplace(emplacedID, std::move(texture));
	return emplacedID;
}

/// <summary>
/// Draws a sprite on screen
/// </summary>
/// <param name="transform">The position, rotation and scale of the sprite</param>
/// <param name="textureId">The ID of the texture to use</param>
/// <param name="SpriteAnchor">The anchor point of the sprite. It should be a normalized vector2, where (0, 0) is the top-left corner and (1, 1) is the bottom-right corner.</param>
void SfmlRenderer::DrawSprite(const Transform& transform, TextureId textureId, const Vector2 SpriteAnchor)
{
    try
    {
        const auto& texture = textures.at(textureId);
        sf::Sprite sprite(texture);
        const sf::Vector2u textureSize = texture.getSize();
        const Vector2 ancourPixel = { static_cast<float>(textureSize.x) * SpriteAnchor.x, static_cast<float>(textureSize.y) * SpriteAnchor.y };
        sprite.setOrigin(ancourPixel);
        sprite.setPosition(sf::Vector2f(transform.position.x, transform.position.y));
        sprite.setRotation(sf::radians(static_cast<float>(transform.rotation.toRadian())));
        sprite.setScale({ transform.size.x / static_cast<float>(textureSize.x), transform.size.y / static_cast<float>(textureSize.y) });
        window.draw(sprite);
    }
    catch (const std::exception&)
    {
        throw TextureLoadException(std::format("[SfmlRenderer::DrawSprite] Sprite from Texture with id={} could not be created!", textureId));
    }
}


void SfmlRenderer::UnloadTexture(TextureId textureId)
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
