#include "Game.h"

// constructor with dependencies
Game::Game(IRenderer& renderer,
	ISceneLoader& sceneLoader,
	IInputService& inputService,
	ILogger& logger) :
	Renderer(renderer),
	SceneLoader(sceneLoader),
	InputService(inputService),
	Logger(logger),
	Running(false) {}

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

bool Game::IsRunning() const {
	return Running;
}

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
}

void Game::Render()
{
	Renderer.Clear();
	Renderer.DrawCircle(mousePosition, 50.0f);
	Renderer.Render();
}

/// <summary>
/// Called once per game loop. Calls the different stages of the game loop
/// </summary>
void Game::RunMainLoop()
{
	HandleEvents();
	HandleInput();
	Render();
}

void Game::Shutdown()
{
	Renderer.Shutdown();
	Logger.Log("Renderer shutdown.");
	Running = false;
}
