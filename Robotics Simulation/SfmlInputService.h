#pragma once

#include "Vector2.h"
#include "IInputService.h"
#include "SfmlRenderer.h"
#include <SFML/Window.hpp>

/// <summary>
/// Easy input service to use if an SFML renderer is used.
/// using this before the renderer is initialized will cause a null-reference exception.
/// </summary>
class SfmlInputService : public IInputService
{
public:
	// Constructor that takes a reference to the SfmlRenderer
	explicit SfmlInputService(SfmlRenderer& sfmlWindowProvider);

	// implementation of IInputService methods
	bool IsKeyPressed(KeyCode key) override;
	Vector2 GetMousePosition() override;
	void HandleEvents() override;
	bool ShouldTerminate() override;

private:
	void verifyWindowInitialized();
	sf::RenderWindow& window; // Reference to the SFML window
	sf::Keyboard::Key KeyToSfmlKeycode(KeyCode key);
	bool ShouldClose = false;
};

