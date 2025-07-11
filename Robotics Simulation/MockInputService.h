#pragma once
#include "IInputService.h"

// a mock class implementing the IInputService interface, without any dependencies
class MockInputService : public IInputService
{
public:
	MockInputService() = default;
	~MockInputService() override = default;
	// Check if a key is pressed
	bool IsKeyPressed(KeyCode key) override {
		return false; // No keys pressed in mock
	}
	// Check if a key is released
	bool IsKeyReleased(KeyCode key) override {
		return false; // No keys released in mock
	}
	// Get the current mouse position
	Vector2 GetMousePosition() override {
		return Vector2(0, 0); // Default position in mock
	}
	// Check if the application should terminate
	bool ShouldTerminate() override {
		return false; // Mock does not terminate
	}
};

