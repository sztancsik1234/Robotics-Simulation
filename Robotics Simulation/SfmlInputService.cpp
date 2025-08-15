#include "SfmlInputService.h"
#include "pch.h"

SfmlInputService::SfmlInputService(SfmlRenderer& sfmlWindowProvider) :
	window(sfmlWindowProvider.window)
{
}

/// <summary>
/// Checks if the specified key is currently pressed.
/// </summary>
/// <param name="key">The key code to check.</param>
/// <returns>true if the specified key is pressed; otherwise, false.</returns>
bool SfmlInputService::IsKeyPressed(KeyCode key)
{
	return sf::Keyboard::isKeyPressed(KeyToSfmlKeycode(key));
}
	
sf::Keyboard::Key SfmlInputService::KeyToSfmlKeycode(KeyCode key)
{
	sf::Keyboard::Key sfmlKey;
	switch (key)
	{
		case KeyCode::UP: return sf::Keyboard::Key::Up;
		case KeyCode::DOWN: return  sf::Keyboard::Key::Down;
		case KeyCode::LEFT:	return sf::Keyboard::Key::Left;
		case KeyCode::RIGHT: return sf::Keyboard::Key::Right;
		case KeyCode::SPACE: return sf::Keyboard::Key::Space;
		case KeyCode::ESCAPE: return sf::Keyboard::Key::Escape;
		case KeyCode::ENTER: return sf::Keyboard::Key::Enter;
		default: throw WrongKeyCodeException(std::to_string(static_cast<int>(key)));
	}
}


/// <summary>
/// get the current mouse position in the window.
/// </summary>
/// <returns>the mouse position in window pixels. origin is top left</returns>
Vector2 SfmlInputService::GetMousePosition()
{
	sf::Vector2i position = sf::Mouse::getPosition(window);
	return Vector2(static_cast<float>(position.x), static_cast<float>(position.y));
}

void SfmlInputService::HandleEvents()
{
	verifyWindowInitialized();
	while (const std::optional event = window.pollEvent())
	{
		if (event.value().is<sf::Event::Closed>())
		{
			ShouldClose = true; // Set the termination flag if the window is closed
		}
		// Handle other events as needed
	}
}

bool SfmlInputService::ShouldTerminate()
{
	return ShouldClose;
}

void SfmlInputService::verifyWindowInitialized()
{

}
