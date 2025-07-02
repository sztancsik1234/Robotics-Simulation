#include "Game.h"
#include "SfmlRenderer.h"
#include "SceneLoader.h"

int main()
{
	//instantiate the renderer and scene loader
	SfmlRenderer renderer;
	SceneLoader sceneLoader;

	//create the game object with dependencies
	Game game(renderer, sceneLoader);

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