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

}

void Game::Update()
{
}

void Game::Render()
{
}

/// <summary>
/// Called once per game loop. Calls the different stages of the game loop
/// </summary>
void Game::RunLoop()
{
	Renderer.DrawCircle({ 100, 100 }, 50.0f);
}

void Game::Shutdown()
{
}
