#pragma once

#include "IRenderer.h"
#include "ILogger.h"
#include "GameObject.h"
#include "Vector2.h"
#include <string>




// Mock GameObject for testing
class MockGameObject : public GameObject {
public:
    MockGameObject(ILogger& logger) : GameObject(logger) {}
};