#include "Game.h"
#include "SfmlRenderer.h"
#include "SceneLoader.h"
#include "SfmlInputService.h"
#include "ConsoleLogger.h"

int main()
{
	//instantiate the renderer and scene loader
	SfmlRenderer	 renderer;
	SceneLoader		 sceneLoader;
	SfmlInputService inputService(renderer);
	ConsoleLogger	 logger;

	//create the game object with dependencies
	Game game(renderer, sceneLoader, inputService, logger);

	//initialize the game
	game.Initialize();

	//run the game loop
	while (game.IsRunning())
	{
		game.RunMainLoop();
	}
	//shutdown the game
	game.Shutdown();
}