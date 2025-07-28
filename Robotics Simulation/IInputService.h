#pragma once
#include "Vector2.h"

// enum for keycodes
enum KeyCode : short
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
	ESCAPE,
	ENTER,
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
};