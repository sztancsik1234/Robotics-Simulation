#pragma once
#include "core/Vector2.h"
#include "input/InputPublisher.h"

// enum for keycodes
enum class KeyCode : char
{
	UP = (char)1,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
	ESCAPE,
	ENTER,
	LEFT_MOUSE_BUTTON,
	// Add more key codes as needed
};

// interface for basic input handling. Also for terminate signal
struct IInputService
{
	virtual ~IInputService() = default;
	// Check if a key is pressed
	virtual bool IsKeyPressed(KeyCode key) = 0;
	// Get the current mouse position
	virtual Vector2 GetMousePosition() = 0;
	// Check if the application should terminate
	virtual bool ShouldTerminate() = 0;
	// Handle events if needed
	virtual void HandleEvents() = 0;

	InputPublisher<ClickEvent> mouseClickSubject;
	InputPublisher<MouseMoveEvent> mouseMoveSubject;
};