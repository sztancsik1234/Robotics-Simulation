#pragma once

#include <forward_list>
#include "graphics/IRenderer.h"
#include "util/ISceneLoader.h"
#include "input/IInputService.h"
#include "util/ILogger.h"
#include "core/GameObject.h"

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

	// TODO: remove test variables
	// TEST variable. Temporary
	std::forward_list<GameObject> gameObjects;

	/// <summary>
	/// Handles input events by delegating to the input service.
	/// </summary>
	void HandleEvents();

	/// <summary>
	/// Handles user input and flips the `running` flag, if termination is requested.
	/// TODO: Update as more functionality is added.
	/// Also updates the mouse position.
	/// </summary>
	void HandleInput();
	// go over all game objects and call their update method
	void Update();
	// clear the frame by clearing the renderer
	void ClearFrame();
	// display what has been drawn so far in the current frame
	void DisplayFrame();

	void addGameObject(GameObject&& gameObject);
	// void addGameObject(GameObject gameObject);

	void clearGameObjects();

	void InitializeRenderer();
	void addTestGameObject();
	void LoadInitialScene();
};

