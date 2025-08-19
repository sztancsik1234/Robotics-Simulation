#pragma once

#include <forward_list>

#include "IRenderer.h"
#include "ISceneLoader.h"
#include "IInputService.h"
#include "ILogger.h"
#include "GameObject.h"

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

	// TEST variable. Temporary
	Vector2 mousePosition;
	// TEST variable. Temporary
	GameObject* testGameObject = nullptr;
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
};

