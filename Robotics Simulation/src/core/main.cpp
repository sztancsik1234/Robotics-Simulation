#include "core/Game.h"
#include "graphics/SfmlRenderer.h"
#include "input/SfmlInputService.h"
#include "util/ConsoleLogger.h"
#include "util/FileLogger.h"

int main()
{
	//instantiate the renderer and scene loader
	ConsoleLogger	 logger(LogLevel::INFO);
	SfmlRenderer	 renderer(logger);
	SfmlInputService inputService(renderer.GetWindow());
	//FileLogger 		 fileLogger("logs", LogLevel::TRACE);

	//create the game object with dependencies
	Game game(renderer, inputService, logger);

	//initialize the game
	game.Initialize();

	try
	{
		game.VerifyState();

	}
	catch (const GameInitializationException&)
	{
		logger.Log("[Main] Game initialization failed. Exiting.", LogLevel::ERROR);
		return -1;
	}

	//run the game loop
	game.StartMainLoop();

	//shutdown the game
	game.Shutdown();
}