#include "core/Game.h"
#include "core/Component.h"
#include "core/Color.inl"
#include "graphics/CircleRenderer.h"

/// <summary>
/// Constructs a Game object with the required dependencies.
/// </summary>
/// <param name="renderer">Reference to the renderer implementation.</param>
/// <param name="sceneLoader">Reference to the scene loader implementation.</param>
/// <param name="inputService">Reference to the input service implementation.</param>
/// <param name="logger">Reference to the logger implementation.</param>
Game::Game(IRenderer& renderer,
	ISceneLoader& sceneLoader,
	IInputService& inputService,
	ILogger& logger) :
	Running(false),
	Renderer(renderer),
	SceneLoader(sceneLoader),
	InputService(inputService),
	Logger(logger) {}

/// <summary>
/// Initializes the game by initializing the renderer and setting the running state.
/// Logs the result of the initialization.
/// </summary>
void Game::Initialize()
{
	InitializeRenderer();

	addCircleRenderer();

	// log the test game object details
	if (testGameObject)
	{
		Logger.Log("Test GameObject ID: " + std::to_string(testGameObject->GetId()), LogLevel::TRACE);
		Logger.Log("Test GameObject Name: " + testGameObject->GetName(), LogLevel::TRACE);
	}
	else
	{
		Logger.Log("No test GameObject available.", LogLevel::WARNING);
	}
}

void Game::InitializeRenderer()
{
	try
	{
		Renderer.Initialize();
		Logger.Log("Renderer initialized.");
		Running = true;
	}
	catch (const std::exception& ex)
	{
		Logger.Log(std::string("Renderer initialization failed: ") + ex.what(), LogLevel::ERROR);
		Running = false;
	}
}

// Adds a CircleRenderer component to a new GameObject and adds it to the game.
void Game::addCircleRenderer()
{
	// create a gameobject, attach a circlerenderer, then add it to the game
	GameObject gameObject(Logger, 1, "CircleRendererObject");
	gameObject.EmplaceComponent<CircleRenderer>(Renderer, Logger);
	addGameObject(std::move(gameObject));
	// TODO: Remove this test game object after testing is done
	testGameObject = &gameObjects.front(); // store the first game object for testing purposes
	Logger.Log("Game initialized with a test GameObject:", LogLevel::TRACE);
}

/// <summary>
/// Returns whether the game is currently running.
/// </summary>
/// <returns>True if the game is running, false otherwise.</returns>
bool Game::IsRunning() const {
	return Running;
}

/// <summary>
/// Handles input events by delegating to the input service.
/// </summary>
void Game::HandleEvents()
{
	InputService.HandleEvents();
}


void Game::HandleInput()
{
	if (InputService.ShouldTerminate())
	{
		Logger.Log("Termination requested. Exiting game.");
		Running = false;
		return;
	}
	if (InputService.IsKeyPressed(KeyCode::ESCAPE))
	{
		Logger.Log("ESCAPE key pressed. Exiting game.");
		Running = false;
	}
	mousePosition = InputService.GetMousePosition();
}

void Game::Update()
{
	// for now, manually update the component position to the mouse position
	testGameObject->SetPosition(mousePosition);

	//iterate through game objects and update them
	for (auto& gameObject : gameObjects)
	{
		Logger.Log("In Game::Update:\tGameobject.position: (" + std::to_string(gameObject.GetPosition().x) + ", " + std::to_string(gameObject.GetPosition().y) + ")", LogLevel::TRACE);
		gameObject.Update();
	}

}

void Game::ClearFrame()
{
	Renderer.Clear(Color::Black);
}

void Game::DisplayFrame()
{
	Renderer.DisplayFrame();
}

void Game::addGameObject(GameObject&& gameObject)
{
	gameObjects.push_front(std::move(gameObject));
	Logger.Log("GameObject added with move semantics.");
}

//void Game::addGameObject(GameObject gameObject)
//{
//	gameObjects.push_front(std::move(gameObject));
//	Logger.Log("GameObject added.");
//}

/// <summary>
/// Starts the main game loop. Returns when the game closes.
/// </summary>
void Game::RunMainLoop()
{
	while (Running)
	{
		ClearFrame();
		HandleEvents();
		HandleInput();
		Update();
		DisplayFrame();
	}
}

void Game::clearGameObjects()
{
	gameObjects.clear();
	Logger.Log("All game objects cleared.");
}

/// <summary>
/// Shuts down the game by shutting down the renderer and logging the shutdown.
/// </summary>
void Game::Shutdown()
{
	clearGameObjects(); // clear all game objects
	Renderer.Shutdown();
	Logger.Log("Renderer shutdown.");

	Running = false;
}
