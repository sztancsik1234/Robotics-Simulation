#include "core/Game.h"
#include "core/Component.h"
#include "core/Color.inl"
#include "graphics/SpriteRendererComponent.h"
#include "graphics/CircleRenderer.h"
#include "input/MouseFollowerComponent.h"
#include "tinyxml/tinyxml2.h"

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

void Game::LoadInitialScene()
{
	// TODO: Load from XML file and load the scene based on that
	// load background
	GameObject background(Logger, 1, "Background");												// TODO: Get the window size from the renderer, and use that for background size
	background.EmplaceComponent<SpriteRenderComponent>(Renderer, Logger, "assets\\Concrete.jpg", Vector2{ 800.f, 800.f });

	GameObject Crate(Logger, 2, "Crate");
	Crate.EmplaceComponent<SpriteRenderComponent>(Renderer, Logger, "assets\\Crate.png", Vector2{ 100.f, 100.f }, Vector2{0.5f, 0.5f});
	Crate.EmplaceComponent<MouseFollowerComponent>(InputService);
	
	addGameObject(std::move(Crate));
	addGameObject(std::move(background));


}

bool Game::IsRunning() const {
	return Running;
}

void Game::HandleEvents()
{
	// Place of improvement: implement a custom event system, or separate from input.
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

	Running = false;
}
