#include "input/SfmlInputService.h"
#include "util/Exceptions.h"
#include <events/MouseMoveEvent.h>

SfmlInputService::SfmlInputService(sf::RenderWindow& window) :
	Window(window)
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
	
sf::Keyboard::Key SfmlInputService::KeyToSfmlKeycode(KeyCode key) const
{
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
	sf::Vector2i position = sf::Mouse::getPosition(Window);
	return Vector2(static_cast<float>(position.x), static_cast<float>(position.y));
}

// querry the window for events and handle them. Window close event is the only one handled for now
void SfmlInputService::HandleEvents()
{
	VerifyWindowInitialized();
	while (const std::optional event = Window.pollEvent())
	{
		if (event.value().is<sf::Event::Closed>())
		{
			ShouldClose = true; // Set the termination flag if the window is closed
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			// Handle key press events if needed
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			auto mouseEvent = MouseMoveEvent(Vector2 { static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y) });
			mouseMoveBroadcast.notify(&mouseEvent);
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button != sf::Mouse::Button::Left)
				continue; // only handle left mouse button for now
			ClickEvent clickEvent { KeyCode::LEFT_MOUSE_BUTTON, Vector2(static_cast<float>(mouseButtonPressed->position.x), static_cast<float>(mouseButtonPressed->position.y)) };
			mouseClickBroadcast.notify(&clickEvent);

		}
		// Handle other events as needed
	}
}

bool SfmlInputService::ShouldTerminate()
{
	return ShouldClose;
}

void SfmlInputService::subscribeToClick(IObserver<ClickEvent>* observer)
{
	clickPublisher.subscribe(observer);
}

void SfmlInputService::unsubscribeFromClick(IObserver<ClickEvent>* observer)
{
	clickPublisher.unsubscribe(observer);
}

void SfmlInputService::subscribeToMouseMove(IObserver<MouseMoveEvent>* observer)
{
	mouseMovePublisher.subscribe(observer);
}

void SfmlInputService::unsubscribeFromMouseMove(IObserver<MouseMoveEvent>* observer)
{
	mouseMovePublisher.unsubscribe(observer);
}

// not implemented, empty for now
void SfmlInputService::VerifyWindowInitialized()
{

}
