#include "core/Game.h"
#include "core/Component.h"
#include "core/Color.inl"
#include "graphics/SpriteRendererComponent.h"
#include "graphics/CircleRenderer.h"
#include "input/MouseFollowerComponent.h"

/// <summary>
/// Constructs a Game object with the required dependencies.
/// </summary>
/// <param name="renderer">Reference to the renderer implementation.</param>
/// <param name="sceneLoader">Reference to the scene loader implementation.</param>
/// <param name="inputService">Reference to the input service implementation.</param>
/// <param name="logger">Reference to the logger implementation.</param>
Game::Game(IRenderer& renderer,
	ISceneLoader& sceneLoader,
	IInputService& inputService,
	ILogger& logger) :
	Running(false),
	Renderer(renderer),
	SceneLoader(sceneLoader),
	InputService(inputService),
	Logger(logger) {}

/// <summary>
/// Initializes the game by initializing the renderer and setting the running state.
/// Logs the result of the initialization.
/// </summary>
void Game::Initialize()
{
	InitializeRenderer();

	LoadInitialScene();
}

void Game::InitializeRenderer()
{
	try
	{
		Renderer.Initialize();
		Logger.Log("Renderer initialized.");
		Running = true;
	}
	catch (const std::exception& ex)
	{
		Logger.Log(std::string("Renderer initialization failed: ") + ex.what(), LogLevel::ERROR);
		Running = false;
	}
}

void Game::addTestGameObject()
{
	// add a test game object, add a circle renderer, and a mouseFollower component to it
	GameObject testObject(Logger, 1, "TestObject");
	testObject.EmplaceComponent<CircleRenderer>(Renderer, Logger);
	testObject.EmplaceComponent<MouseFollowerComponent>(InputService);
	addGameObject(std::move(testObject));
}

// A temporary function to add a couple of game objects to the game for testing purposes
void Game::LoadInitialScene()
{
	// load background
	GameObject background(Logger, 1, "Background");												// TODO: Get the window size from the renderer, and use that for background size
	background.EmplaceComponent<SpriteRenderComponent>(Renderer, Logger, "assets\\Concrete.jpg", Vector2{ 800.f, 800.f });

	GameObject Crate(Logger, 2, "Crate");
	Crate.EmplaceComponent<SpriteRenderComponent>(Renderer, Logger, "assets\\Crate.png", Vector2{ 100.f, 100.f }, Vector2{0.5f, 0.5f});
	Crate.EmplaceComponent<MouseFollowerComponent>(InputService);
	
	addGameObject(std::move(Crate));
	addGameObject(std::move(background));
}

/// <summary>
/// Returns whether the game is currently running.
/// </summary>
/// <returns>True if the game is running, false otherwise.</returns>
bool Game::IsRunning() const {
	return Running;
}

/// <summary>
/// Handles input events by delegating to the input service.
/// </summary>
void Game::HandleEvents()
{
	InputService.HandleEvents();
}


void Game::HandleInput()
{
	if (InputService.ShouldTerminate())
	{
		Logger.Log("Termination requested. Exiting game.");
		Running = false;
		return;
	}
	if (InputService.IsKeyPressed(KeyCode::ESCAPE))
	{
		Logger.Log("ESCAPE key pressed. Exiting game.");
		Running = false;
	}
}

void Game::Update()
{
	//iterate through game objects and update them
	for (auto& gameObject : gameObjects)
	{
		Logger.Log("In Game::Update:\tGameobject.position: (" + std::to_string(gameObject.GetPosition().x) + ", " + std::to_string(gameObject.GetPosition().y) + ")", LogLevel::TRACE);
		gameObject.Update();
	}

}

void Game::ClearFrame()
{
	Renderer.Clear(Color::Black);
}

void Game::DisplayFrame()
{
	Renderer.DisplayFrame();
}

void Game::addGameObject(GameObject&& gameObject)
{
	gameObjects.push_front(std::move(gameObject));
	Logger.Log("GameObject added with move semantics.");
}

//void Game::addGameObject(GameObject gameObject)
//{
//	gameObjects.push_front(std::move(gameObject));
//	Logger.Log("GameObject added.");
//}

/// <summary>
/// Starts the main game loop. Returns when the game closes.
/// </summary>
void Game::RunMainLoop()
{
	while (Running)
	{
		ClearFrame();
		HandleEvents();
		HandleInput();
		Update();
		DisplayFrame();
	}
}

void Game::clearGameObjects()
{
	gameObjects.clear();
	Logger.Log("All game objects cleared.");
}

/// <summary>
/// Shuts down the game by shutting down the renderer and logging the shutdown.
/// </summary>
void Game::Shutdown()
{
	clearGameObjects(); // clear all game objects
	Renderer.Shutdown();
	Logger.Log("Renderer shutdown.");

	Running = false;
}
