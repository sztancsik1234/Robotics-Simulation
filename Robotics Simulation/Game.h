#pragma once
#include "IRenderer.h"
#include "ISceneLoader.h"
#include "IInputService.h"
#include "ILogger.h"

class Game
{
public:
	Game(
		IRenderer& renderer,
		ISceneLoader& sceneLoader,
		IInputService& inputService,
		ILogger& logger);

	void Initialize();
	void RunMainLoop();
	void Shutdown();
	bool IsRunning() const;

private:
	bool Running;
	IRenderer& Renderer;
	ISceneLoader& SceneLoader;
	IInputService& InputService;
	ILogger& Logger;

	Vector2 mousePosition;

	void HandleEvents();
	void HandleInput();
	void Update();
	void Render();
};

