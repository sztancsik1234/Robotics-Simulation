#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "graphics/IRenderer.h"
#include "core/Color.inl"
#include "util/ILogger.h"

// TODO: Implement camera for translating game coordinates to screen coordinates
class SfmlRenderer : public IRenderer
{
public:
	SfmlRenderer(ILogger& logger);
	~SfmlRenderer() override;

	/// <summary>
	/// Initializes the SFML window with a specific size and title.
	/// Sets the frame rate limit to 60 FPS.
	/// </summary>
	void Initialize() override;

	bool IsInitialized() const override;

	/// <summary>
	/// Provide a reference for the window to other services, like input.
	/// </summary>
	/// <returns>The reference to the window</returns>
	sf::RenderWindow& GetWindow();

	/// <summary>
	/// Clears the window with a black color.
	/// </summary>
	void Clear(Color color) override;

	/// <summary>
	/// Draws a white line between two points.
	/// </summary>
	/// <param name="start">The starting point of the line.</param>
	/// <param name="end">The endpoint of the line.</param>
	void DrawLine(Vector2 x, Vector2 y) override;

	/// <summary>
	/// Draws a circle at the specified position with the given radius.
	/// The circle is outlined in white and filled transparent.
	/// </summary>
	/// <param name="p">The center position of the circle.</param>
	/// <param name="radius">The radius of the circle.</param>
	void DrawCircle(Vector2 p, float radius) override;

	/// <summary>
	/// Draws a rectangle at the specified position with the given width and height.
	/// The rectangle is outlined in white and filled transparent.
	/// </summary>
	/// <param name="p1">The top-left position of the rectangle.</param>
	/// <param name="width">The width of the rectangle.</param>
	/// <param name="height">The height of the rectangle.</param>
	void DrawRectangle(Vector2 p1, float width, float height) override;

	/// <summary>
	/// Draws a rectangle defined by two points (top-left and bottom-right).
	/// The rectangle is outlined in white and filled transparent.
	/// </summary>
	/// <param name="p1">The top-left position of the rectangle.</param>
	/// <param name="p2">The bottom-right position of the rectangle.</param>
	void DrawRectangle(Vector2 p1, Vector2 p2) override;

	/// <summary>
	/// Loads a texture from a file into memory. Most file formats are supported
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns>A textureId identifying the texture. subsequent draw calls use this ID as a parameter.</returns>
	TextureId LoadTexture(const std::string filePath) override;

	/// <summary>
	/// Loads a default texture, for when a texture is needed, but not specified.
	/// </summary>
	/// <returns>A textureId identifying the texture. subsequent draw calls use this ID as a parameter.</returns>
	TextureId LoadTexture() override;

	/// <summary>
	/// Draws a sprite at a specified position with a given texture, scale, and anchor point.
	/// </summary>
	/// <param name="position">The position on the screen where the sprite will be drawn.</param>
	/// <param name="textureId">The identifier of the texture to use for the sprite.</param>
	/// <param name="scale">The size of the drawn sprite in pixels.</param>
	/// <param name="SpriteAnchor">The anchor point for the sprite's origin, specified as a normalized vector. {0,0} for top left corner, {1,1} for bottom right</param>
	void DrawSprite(const Transform& transform, TextureId textureId, const Vector2 SpriteAnchor = { 0.f,0.f }) final;
	
	/// <summary>
	/// Unloads a texture from memory safely.
	/// </summary>
	/// <param name="textureId"></param>
	void UnloadTexture(TextureId textureId) override;
	void DisplayFrame() override;
	void Shutdown() override;

private:
	ILogger& Logger;
	sf::RenderWindow window;
	sf::Clock clock;
	float deltaTime = 0.0f;

	unsigned int TextureKeyCounter = 0;
	std::unordered_map<TextureId, sf::Texture> textures;	// conciderations have been made for the type of container used here. In the end, unordered_map was chosen for its simplicity and overhead. 
															// If performance becomes an issue, raw memory addresses could be used as id, but I pity the guy, who would need to debug that.

	// color to sf::color
	sf::Color ConvertColor(Color color) const;
};

