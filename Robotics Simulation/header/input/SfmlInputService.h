#pragma once

#include "core/Vector2.h"
#include "input/IInputService.h"
#include "graphics/SfmlRenderer.h"

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
	void verifyWindowInitialized();
	sf::RenderWindow& Window; // Reference to the SFML window
	sf::Keyboard::Key KeyToSfmlKeycode(KeyCode key) const;
	bool ShouldClose = false;
};

