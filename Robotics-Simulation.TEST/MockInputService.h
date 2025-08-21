#pragma once
#include "input/IInputService.h"

// Mock input service for testing
class MockInputService : public IInputService {
public:
    // Track method calls
    bool isKeyPressedCalled = false;
    bool getMousePositionCalled = false;
    bool shouldTerminateCalled = false;
    bool handleEventsCalled = false;
    
    // Store last used parameters and return values
    KeyCode lastKeyCode;
    Vector2 mousePosition{0.0f, 0.0f};
    bool keyPressedResult = false;
    bool shouldTerminateResult = false;

    // Implementation of IInputService interface
    bool IsKeyPressed(KeyCode key) override {
        isKeyPressedCalled = true;
        lastKeyCode = key;
        return keyPressedResult;
    }

    Vector2 GetMousePosition() override {
        getMousePositionCalled = true;
        return mousePosition;
    }

    bool ShouldTerminate() override {
        shouldTerminateCalled = true;
        return shouldTerminateResult;
    }

    void HandleEvents() override {
        handleEventsCalled = true;
    }
};