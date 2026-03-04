#pragma once

#include "core/Vector2.h"
#include "input/IInputService.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

/// <summary>
/// Easy input service to use if an SFML renderer is used.
/// <Remark>using this before the renderer is initialized will cause a null-reference exception.</Remark>
/// </summary>
class SfmlInputService : public IInputService
{
public:
	// Constructor that takes a reference to the SfmlRenderer
	explicit SfmlInputService(sf::RenderWindow& window);

	// implementation of IInputService methods

	bool IsKeyPressed(KeyCode key) override;
	Vector2 GetMousePosition() override;
	void HandleEvents() override;
	bool ShouldTerminate() override;

private:
	void VerifyWindowInitialized();
	sf::RenderWindow& Window; // Reference to the SFML window
	sf::Keyboard::Key KeyToSfmlKeycode(KeyCode key) const;
	bool ShouldClose = false;
};

