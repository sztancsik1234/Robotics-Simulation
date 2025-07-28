#include "Game.h"

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
	Renderer(renderer),
	SceneLoader(sceneLoader),
	InputService(inputService),
	Logger(logger),
	Running(false) {}

/// <summary>
/// Initializes the game by initializing the renderer and setting the running state.
/// Logs the result of the initialization.
/// </summary>
void Game::Initialize()
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

/// <summary>
/// Handles user input and updates the running state if termination is requested.
/// Also updates the mouse position.
/// </summary>
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

/// <summary>
/// Updates the game state. (Currently empty.)
/// </summary>
void Game::Update()
{
}

/// <summary>
/// Renders the current game state, including drawing a circle at the mouse position.
/// </summary>
void Game::Render()
{
	Renderer.Clear();
	Renderer.DrawCircle(mousePosition, 50.0f);
	Renderer.Render();
}

/// <summary>
/// Called once per game loop. Calls the different stages of the game loop.
/// </summary>
void Game::RunMainLoop()
{
	HandleEvents();
	HandleInput();
	Render();
}

/// <summary>
/// Shuts down the game by shutting down the renderer and logging the shutdown.
/// </summary>
void Game::Shutdown()
{
	Renderer.Shutdown();
	Logger.Log("Renderer shutdown.");
	Running = false;
}
