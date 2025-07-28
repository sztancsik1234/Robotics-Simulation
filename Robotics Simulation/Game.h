#pragma once
#include "IRenderer.h"
#include "ISceneLoader.h"
#include "IInputService.h"

class Game
{
public:

	// constructor with dependencies
    Game(
        IRenderer& renderer, 
        ISceneLoader& sceneLoader,
        IInputService& inputService);
    void Initialize();
    void RunMainLoop();
    void Shutdown();
    bool IsRunning() const;

private:
    bool Running;
    IRenderer& Renderer;
    ISceneLoader& SceneLoader;
	IInputService& InputService;

    Vector2 mousePosition;

	void HandleEvents();
    void HandleInput();
    void Update();
    void Render();
};

