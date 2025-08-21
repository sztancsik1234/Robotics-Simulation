#include "core/Game.h"
#include "graphics/SfmlRenderer.h"
#include "util/SceneLoader.h"
#include "input/SfmlInputService.h"
#include "util/ConsoleLogger.h"

int main()
{
	//instantiate the renderer and scene loader
	SfmlRenderer	 renderer;
	SceneLoader		 sceneLoader;
	SfmlInputService inputService(renderer);
	ConsoleLogger	 logger(LogLevel::INFO);

	//create the game object with dependencies
	Game game(renderer, sceneLoader, inputService, logger);

	//initialize the game
	game.Initialize();

	//run the game loop
	game.RunMainLoop();

	//shutdown the game
	game.Shutdown();
}