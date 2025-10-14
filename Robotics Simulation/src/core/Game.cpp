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
	IInputService& inputService,
	ILogger& logger) :
	Running(false),
	Renderer(renderer),
	InputService(inputService),
	Logger(logger),
	mainCamera(Camera(
		static_cast<IDrawableRenderer&>(Renderer),
		Logger,
		DEFAULT_SCREEN_SIZE_PIXELS,
		Vector2{ 0.f, 0.f },
		Vector2{ DEFAULT_CAMERA_FOV, DEFAULT_CAMERA_FOV * (DEFAULT_SCREEN_SIZE_PIXELS.y / DEFAULT_SCREEN_SIZE_PIXELS.x) } // Maintain aspect ratio
	))
{
}

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
		Logger.Log("[Game] Renderer initialized.");
	}
	catch (const std::exception& ex)
	{
		Logger.Log(std::string("[Game] Renderer initialization failed: ") + ex.what(), LogLevel::ERROR);
	}
}

void Game::LoadInitialScene()
{
	Scene scn = sceneLoader.LoadScene(INTIAL_SCENE_PATH);
	activeScene = std::make_unique<Scene>(std::move(scn));
}

bool Game::IsRunning() const {
	return Running;
}


void Game::StartMainLoop()
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

void Game::HandleEvents()
{
	// Place of improvement: implement a custom event system, or separate from input.
	InputService.HandleEvents();
}

void Game::HandleInput()
{
	if (InputService.ShouldTerminate())
	{
		Logger.Log("[Game] Termination requested. Exiting game.");
		Running = false;
		return;
	}
	if (InputService.IsKeyPressed(KeyCode::ESCAPE))
	{
		Logger.Log("[Game] ESCAPE key pressed. Exiting game.");
		Running = false;
	}
}

void Game::Update()
{
	//iterate through game objects and update them
	// TODO: Investigate if this is a copy or not. Concider using references if it is.
	auto& gameobjects = activeScene->getGameObjects();
	for (auto& gameObject : gameobjects)
	{
		// TODO: BUG HERE memory violation
		Logger.Log("[Game] In Game::Update:\tGameobject.position: (" + std::to_string(gameObject.GetPosition().x) + ", " + std::to_string(gameObject.GetPosition().y) + ")", LogLevel::TRACE);
		gameObject.Update();
	}

}

void Game::VerifyState()
{
	// verify Renderer and scene are initialized
	if (!Renderer.IsInitialized())
	{
		Logger.Log("[Game] Renderer not initialized.", LogLevel::ERROR);
		Running = false;
		throw GameInitializationException("Renderer not initialized.");
	}
	if (!activeScene)
	{
		Logger.Log("[Game] No active scene loaded.", LogLevel::ERROR);
		Running = false;
		throw GameInitializationException("No active scene loaded.");
	}
	Logger.Log("[Game] Game is ready to run.", LogLevel::INFO);
	Running = true;
	return;
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
	activeScene->addGameObject(std::move(gameObject));
	Logger.Log("[Game] GameObject added with move semantics.");
}


/// <summary>
/// Shuts down the game by shutting down the renderer and logging the shutdown.
/// </summary>
void Game::Shutdown()
{
	activeScene->Unload(); // Unload scene to clear all game objects
	Renderer.Shutdown();

	Running = false;
}


void Game::addTestGameObject()
{
	// add a test game object, add a circle renderer, and a mouseFollower component to it
	GameObject testObject(Logger, 1, { 0.f, 0.f }, "TestObject");
	testObject.EmplaceComponent<CircleRenderer>(*mainCamera, Logger);
	testObject.EmplaceComponent<MouseFollowerComponent>(InputService);
	addGameObject(std::move(testObject));
}
