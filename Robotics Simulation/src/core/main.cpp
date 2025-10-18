#include "core/Game.h"
#include "graphics/SfmlRenderer.h"
#include "input/SfmlInputService.h"
#include "util/ConsoleLogger.h"
#include "util/FileLogger.h"
#include "physics/b2Physics.h"

int main()
{
	//instantiate the renderer and scene loader
	ConsoleLogger	 logger(LogLevel::TRACE);
	SfmlRenderer	 renderer(logger);
	SfmlInputService inputService(renderer.GetWindow());
	b2Physics		 physicsEngine(logger);
	//FileLogger 		 fileLogger("logs", LogLevel::TRACE);

	//create the game object with dependencies
	Game game(physicsEngine, renderer, inputService, logger);

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