#include "core/Game.h"
#include "graphics/SfmlRenderer.h"
#include "util/SceneLoader.h"
#include "input/SfmlInputService.h"
#include "util/ConsoleLogger.h"
#include "util/FileLogger.h"

int main()
{
	//instantiate the renderer and scene loader
	ConsoleLogger	 logger(LogLevel::INFO);
	SfmlRenderer	 renderer(logger);
	SceneLoader		 sceneLoader;
	SfmlInputService inputService(renderer);
	//FileLogger 		 fileLogger("logs", LogLevel::TRACE);

	//create the game object with dependencies
	Game game(renderer, sceneLoader, inputService, logger);

	//initialize the game
	game.Initialize();

	//run the game loop
	game.RunMainLoop();

	//shutdown the game
	game.Shutdown();
}