#include "core/Scene.h"

Scene::Scene(std::string path) : 
	SceneFilePath(std::move(path)),
    gameObjects()
{
}

Scene::Scene(Scene&& other) noexcept
    : gameObjects(std::move(other.gameObjects))
    // b2World is not copyable; rebuild with the same gravity when moving.
{
    // Note: Bodies/joints are not transferred. If needed, re-create them here.
}

Scene& Scene::operator=(Scene&& other) noexcept
{
    if (this == &other) return *this;

    // TODO: test this.
    gameObjects = std::move(other.gameObjects);

    return *this;
}

void Scene::LogGameObjects(ILogger& logger, bool logComonents) const
{
    std::string msgToLog = "[Scene] Active gameobjects:\n";
    for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
		msgToLog += std::format("\t{}", it->ToString(logComonents));
    }
	logger.Log(msgToLog, LogLevel::INFO);
}

void Scene::OnLoad()
{
	// If OnSceneLoad is implemented for components, call it here for each component of each game object.
}

void Scene::OnUnload()
{
    // If OnSceneUnload is implemented for components, call it here for each component of each game object.

	ClearGameObjects();
}

void Scene::Unload()
{
	OnUnload();
	gameObjects.clear();
}

void Scene::AddGameObject(GameObject&& gameObject)
{
    gameObjects.push_front(std::move(gameObject));
}

void Scene::ClearGameObjects()
{
    gameObjects.clear();
}