#pragma once
#include "IRenderer.h"
#include "ISceneLoader.h"

class Game
{
public:

	// constructor with dependencies
    Game(IRenderer& renderer, ISceneLoader& sceneLoader);
    void Initialize();
    void RunLoop();
    void Shutdown();
    bool IsRunning() const;

private:
    bool Running;
    IRenderer& Renderer;
    ISceneLoader& SceneLoader;

    void ProcessInput();
    void Update();
    void Render();
};

