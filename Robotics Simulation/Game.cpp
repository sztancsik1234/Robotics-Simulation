#include "Game.h"

// constructor with dependencies
Game::Game(IRenderer& renderer, ISceneLoader& sceneLoader)
	: Renderer(renderer), SceneLoader(sceneLoader), Running(false) {
}

void Game::Initialize()
{
	throw NotImplementedException();
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

void Game::RunLoop()
{
}

void Game::Shutdown()
{
}
