#include "core/Game.h"
#include "core/Component.h"
#include "core/Color.inl"
#include "graphics/SpriteRendererComponent.h"
#include "graphics/CircleRendererComponent.h"
#include "input/MouseFollowerComponent.h"
#include "tinyxml/tinyxml2.h"
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define FIXED_DELTA_TIME
#endif // _DEBUG


// #define FIXED_DELTA_TIME

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
	PhysicsEngine(physicsEngine),
	Renderer(renderer),
	InputService(inputService),
	Logger(logger),
	Running(false),
	mainCamera(
		Viewport( Vector2{0.f, 0.f}, DEFAULT_SCREEN_SIZE_PIXELS, Vector2{ DEFAULT_CAMERA_FOV, DEFAULT_CAMERA_FOV * (DEFAULT_SCREEN_SIZE_PIXELS.y / DEFAULT_SCREEN_SIZE_PIXELS.x) }),
		static_cast<IDrawableRenderer&>(Renderer),
		&logger
	),
	uiCamera(
		IdentityViewport(),
		static_cast<IDrawableRenderer&>(Renderer),
		&logger
	),
	messageDispatcher(CentralMessageDispatcher())
{
}

void Game::Initialize()
{
	InitializeSettings();
	InitializePhysicsEngine();
	InitializeRenderer();
	LoadInitialScene();
}

void Game::InitializeSettings()
{
	std::ifstream file(settingsPath);

	if (!file.is_open())
	{
		throw std::runtime_error("[Game] Failed to open settings file: " + settingsPath);
	}

	try
	{
		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '#')
				continue;

			const auto delimPos = line.find('=');
			if (delimPos == std::string::npos)
				continue;

			const std::string key = line.substr(0, delimPos);
			const std::string value = line.substr(delimPos + 1);

			if (key == "initialScene")
			{
				initialScenePath = value;
				Logger.Log("[Game] Initial scene path set to: " + initialScenePath);
			}
		}
	}
	catch (...)
	{
		file.close();
		throw;
	}

	file.close();
}

void Game::InitializeRenderer()
{
	try
	{
		Renderer.Initialize(DEFAULT_SCREEN_SIZE_PIXELS);
	}
	catch (const std::exception& ex)
	{
		Logger.Log(std::string("[Game] Renderer initialization failed: ") + ex.what(), LogLevel::ERROR);
		return;
	}
	Logger.Log("[Game] Renderer initialized.");
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
	// TODO: null-check initialscenepath
	Scene scn = sceneLoader.LoadScene(initialScenePath);
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
		UpdatePhysics();
		DisplayFrame();
#ifdef _DEBUG
		Logger.Log(std::format("\n-----------------------\n[Game] Frame completed. Delta time: {:.4f} seconds\n-----------------------", deltaSeconds), LogLevel::TRACE);
#endif // _DEBUG

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
	auto& gameobjects = activeScene->GetGameObjects();
	auto& uiElements = activeScene->GetUiGameObjects();
	for (auto& gameObject : gameobjects)
	{
		Logger.Log(std::format("[Game] Updating '{}'", gameObject.ToString()), LogLevel::TRACE);
		gameObject.Update();
	}

	for (auto& uiGameObject : uiElements)
	{
		Logger.Log(std::format("[Game] Updating UI '{}'", uiGameObject.ToString()), LogLevel::TRACE);
		uiGameObject.Update();
	}

}

void Game::UpdatePhysics()
{
	// 0 for debug purposes. Change to 1 when in running nominally
#ifdef FIXED_DELTA_TIME
	PhysicsEngine.simulateStep(0.2f);
#else
	PhysicsEngine.simulateStep(deltaSeconds);
#endif
}

void Game::AssertGameReady()
{
	// verify Renderer and scene are initialized
	if (!Renderer.IsInitialized())
	{
		Logger.Log("[Game] Renderer not initialized.", LogLevel::ERROR);
		Running = false;
		throw GameInitializationException("Renderer not initialized.");
	}

	if (!PhysicsEngine.IsInitialized())
	{
		Logger.Log("[Game] Physics engine not initialized.", LogLevel::ERROR);
		Running = false;
		throw GameInitializationException("Physics engine not initialized.");
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
	using namespace std::chrono; 
	const auto now = steady_clock::now();
	deltaSeconds = duration<float>(now - lastFrameTime).count();
	lastFrameTime = now;
}

void Game::AddGameObject(GameObject&& gameObject)
{
	activeScene->AddGameObject(std::move(gameObject));
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


void Game::AddTestGameObject()
{
	// add a test game object, add a circle renderer, and a mouseFollower component to it
	GameObject testObject(Logger, 1, { 0.f, 0.f }, "TestObject");
	testObject.EmplaceComponent<CircleRendererComponent>(mainCamera, Logger);
	testObject.EmplaceComponent<MouseFollowerComponent>(static_cast<const IViewport&>(mainCamera.GetViewport()), InputService);
	AddGameObject(std::move(testObject));
}
