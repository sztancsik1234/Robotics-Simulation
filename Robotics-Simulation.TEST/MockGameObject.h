#pragma once
#include "core/GameObject.h"

// Mock GameObject for testing
class MockGameObject : public GameObject {
public:
    MockGameObject(ILogger& logger) : GameObject(logger) {}
};