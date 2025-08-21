#pragma once

#include <string>
#include "graphics/IRenderer.h"
#include "util/ILogger.h"
#include "core/GameObject.h"
#include "core/Vector2.h"




// Mock GameObject for testing
class MockGameObject : public GameObject {
public:
    MockGameObject(ILogger& logger) : GameObject(logger) {}
};