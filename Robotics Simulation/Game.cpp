#include "Game.h"

// constructor with dependencies
Game::Game(IRenderer& renderer, 
	ISceneLoader& sceneLoader, 
	IInputService& inputService) : 
		Renderer(renderer), 
		SceneLoader(sceneLoader), 
		InputService(inputService), 
		Running(false) {}

void Game::Initialize()
{
	// initialize the renderer
	Renderer.Initialize();

	
	Running = true;
}

bool Game::IsRunning() const {
	return Running;
}

void Game::ProcessInput()
{
	if (InputService.ShouldTerminate())
	{
		Running = false;
		return;
	}
	// Process input events
	if (InputService.IsKeyPressed(KeyCode::ESCAPE))
	{
		Running = false; // Exit the game if ESCAPE is pressed
	}
	// Add more input processing as needed
	Vector2 mousePosition = InputService.GetMousePosition();
	// Use mousePosition for game logic if needed

}

void Game::Update()
{
}

void Game::Render()
{
	Renderer.Clear();
	Renderer.DrawCircle(Vector2(100, 100), 50.0f);
	Renderer.Render();
}

/// <summary>
/// Called once per game loop. Calls the different stages of the game loop
/// </summary>
void Game::RunLoop()
{
	ProcessInput();
	Render();
}

void Game::Shutdown()
{
	Renderer.Shutdown();
	Running = false; // Set running to false to stop the game loop
}
