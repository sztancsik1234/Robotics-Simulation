#pragma once

#include <forward_list>
#include "graphics/IRenderer.h"
#include "input/IInputService.h"
#include "util/ILogger.h"
#include "core/GameObject.h"

class Game
{
public:
	Game(
		IRenderer& renderer,
		IInputService& inputService,
		ILogger& logger);

	/// <summary>
	/// Initializes the game by initializing the renderer and setting the running state.
	/// Logs the result of the initialization.
	/// </summary>
	void Initialize();

	/// <summary>
	/// Starts the main game loop. Returns when the game closes.
	/// </summary>
	void RunMainLoop();
	void Shutdown();

	/// <summary>
	/// Returns whether the game is currently running.
	/// </summary>
	/// <returns>True if the game is running, false otherwise.</returns>
	bool IsRunning() const;

private:
	bool Running;
	IRenderer& Renderer;
	IInputService& InputService;
	ILogger& Logger;

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

	/// <summary>
	/// Add a new gameobject to the active scene
	/// </summary>
	/// <remark> The method uese move scemantics to move a valid gameobject into the buffer.</remark>
	/// <param name="gameObject"></param>
	void addGameObject(GameObject&& gameObject);

	// Clear all gameobjects, destroying them cleanly
	void clearGameObjects();

	// Initializes everything needed for rendering. Called on initialization.
	void InitializeRenderer();

	// loads the starup scene. For now, hardcoded, but later use scenemanager.
	void LoadInitialScene();

	// A test function to load a simple component with a circleRenderer
	void addTestGameObject();
};

