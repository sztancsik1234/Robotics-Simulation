#include "Game.h"
#include "SfmlRenderer.h"
#include "SceneLoader.h"
#include "SfmlInputService.h"

int main()
{
	//instantiate the renderer and scene loader
	SfmlRenderer	 renderer;
	SceneLoader		 sceneLoader;
	SfmlInputService inputService(renderer);

	//create the game object with dependencies
	Game game(renderer, sceneLoader, inputService);

	//initialize the game
	game.Initialize();

	//run the game loop
	while (game.IsRunning())
	{
		game.RunLoop();
	}
	//shutdown the game
	game.Shutdown();
}