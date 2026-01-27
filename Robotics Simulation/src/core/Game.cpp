#include "core/Game.h"
#include "core/Component.h"
#include "core/Color.inl"
#include "graphics/SpriteRendererComponent.h"
#include "graphics/CircleRendererComponent.h"
#include "input/MouseFollowerComponent.h"
#include "tinyxml/tinyxml2.h"

/// <summary>
/// Constructs a Game object with the required dependencies.
/// </summary>
/// <param name="renderer">Reference to the renderer implementation.</param>
/// <param name="sceneLoader">Reference to the scene loader implementation.</param>
/// <param name="inputService">Reference to the input service implementation.</param>
/// <param name="logger">Reference to the logger implementation.</param>
Game::Game(
	IPhysicsEngine& physicsEngine,
	IRenderer& renderer,
	IInputService& inputService,
	ILogger& logger) :
	Running(false),
	PhysicsEngine(physicsEngine),
	Renderer(renderer),
	InputService(inputService),
	Logger(logger),
	mainCamera(
		static_cast<IDrawableRenderer&>(Renderer),
		Logger,
		Vector2{ 0.f, 0.f },
		DEFAULT_SCREEN_SIZE_PIXELS,
		Vector2{ DEFAULT_CAMERA_FOV, DEFAULT_CAMERA_FOV * (DEFAULT_SCREEN_SIZE_PIXELS.y / DEFAULT_SCREEN_SIZE_PIXELS.x) } // Maintain aspect ratio
	)
{
}

void Game::Initialize()
{
	InitializePhysicsEngine();
	InitializeRenderer();
	LoadInitialScene();
}

void Game::InitializeRenderer()
{
	try
	{
		Renderer.Initialize(DEFAULT_SCREEN_SIZE_PIXELS);
		Logger.Log("[Game] Renderer initialized.");
	}
	catch (const std::exception& ex)
	{
		Logger.Log(std::string("[Game] Renderer initialization failed: ") + ex.what(), LogLevel::ERROR);
	}
}

void Game::InitializePhysicsEngine()
{
	try
	{
		PhysicsEngine.Initialize();
		Logger.Log("[Game] Physics engine initialized.");
	}
	catch (const std::exception& ex)
	{
		Logger.Log(std::string("[Game] Physics engine initialization failed: ") + ex.what(), LogLevel::ERROR);
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
	// Initialize the frame timer
	lastFrameTime = std::chrono::steady_clock::now();

	while (Running)
	{
		UpdateDeltaTime();
		ClearFrame();
		HandleEvents();
		HandleInput();
		UpdateGameObjects();
		updatePhysics();
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

void Game::UpdateGameObjects()
{
	//iterate through game objects and update them
	// TODO: Investigate if this is a copy or not. Concider using references if it is.
	auto& gameobjects = activeScene->getGameObjects();
	for (auto& gameObject : gameobjects)
	{
		Logger.Log(std::format("[Game] Updating '{}'", gameObject.ToString()), LogLevel::TRACE);
		gameObject.Update();
	}

}

void Game::updatePhysics()
{
	PhysicsEngine.simulateStep(deltaSeconds);
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

void Game::UpdateDeltaTime()
{
	using clock = std::chrono::steady_clock;
	const auto now = clock::now();
	deltaSeconds = std::chrono::duration<float>(now - lastFrameTime).count();
	lastFrameTime = now;
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
	PhysicsEngine.Shutdown();
	Renderer.Shutdown();

	Running = false;
}


void Game::addTestGameObject()
{
	// add a test game object, add a circle renderer, and a mouseFollower component to it
	GameObject testObject(Logger, 1, { 0.f, 0.f }, "TestObject");
	testObject.EmplaceComponent<CircleRendererComponent>(mainCamera, Logger);
	testObject.EmplaceComponent<MouseFollowerComponent>(mainCamera, InputService);
	addGameObject(std::move(testObject));
}
