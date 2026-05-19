#include "core/Scene.h"

Scene::Scene(std::string path) : 
	SceneFilePath(std::move(path)),
    gameObjects()
{
}

Scene::Scene(Scene&& other) noexcept
    : gameObjects(std::move(other.gameObjects)),
    uiGameObjects(std::move(other.uiGameObjects))
    // b2World is not copyable; rebuild with the same gravity when moving.
{
    // Note: Bodies/joints are not transferred. If needed, re-create them here.
}

Scene& Scene::operator=(Scene&& other) noexcept
{
    if (this == &other) return *this;

    gameObjects = std::move(other.gameObjects);
	uiGameObjects = std::move(other.uiGameObjects);
	markedForDelete = std::move(other.markedForDelete);
    uiMarkedForDelete = std::move(other.uiMarkedForDelete);

    return *this;
}

void Scene::LogGameObjects(ILogger& logger, bool logComonents) const
{
    std::string msgToLog = "[Scene] Active gameobjects:\n";
    for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
    {
		msgToLog += std::format("\t{}", it->ToString(logComonents));
    }
    msgToLog += "\nActive UI elements:\n";
    for (auto it = uiGameObjects.begin(); it != uiGameObjects.end(); it++)
    {
        msgToLog += std::format("\t{}", it->ToString(logComonents));
    }
	logger.Log(msgToLog, LogLevel::INFO);
}

void Scene::OnLoad()
{
	// If OnSceneLoad is implemented for gameObjects, call it here for each component of each game object.
}

void Scene::OnUnload()
{
    // If OnSceneUnload is implemented for gameObjects, call it here for each component of each game object.

	ClearGameObjects();
}

void Scene::Unload()
{
	OnUnload();
    OnRemoveGameObjects();
	gameObjects.clear();
}

// Call the OnRemove function for every game object
void Scene::OnRemoveGameObjects()
{
    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        it->OnRemove();

    }
    for (auto it = uiGameObjects.begin(); it != uiGameObjects.end(); ++it)
    {
        it->OnRemove();
    }
}

GameObject* Scene::MoveGameObject(GameObject&& gameObject)
{
    gameObjects.push_back(std::move(gameObject));
    return &gameObjects.back();
}

GameObject* Scene::AddGameObject(const GameObject& gameObject)
{
    gameObjects.push_back(gameObject);
    return &gameObjects.back();
}

GameObject* Scene::AddUiGameObject(GameObject&& uiGameObject)
{
	uiGameObjects.push_back(std::move(uiGameObject));
    return &uiGameObjects.back();
}

void Scene::ClearGameObjects()
{
	uiGameObjects.clear();
    gameObjects.clear();
}

void Scene::UpdateGameObjects()
{
    for (auto& gameObject : gameObjects)
    {
        gameObject.Update();
    }
    for (auto& uiElements : uiGameObjects)
    {
        uiElements.Update();
    }
    DestroyDeletedGameObjects();
}

void Scene::RemoveGameObject(GameObject* goPtr)
{
    for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        if (std::to_address(it) == goPtr)
        {
            it->OnRemove();
            markedForDelete.push_back(it);
            return;
        }
    }
    for (auto it = uiGameObjects.begin(); it != uiGameObjects.end(); ++it)
    {
        if (std::to_address(it) == goPtr)
        {
            it->OnRemove();
            uiMarkedForDelete.push_back(it);
            return;
        }
    }
}

void Scene::DestroyDeletedGameObjects()
{
    for (auto const& go : markedForDelete)
    {
        gameObjects.erase(go);
    }

    markedForDelete.clear();
    for (auto const& go : uiMarkedForDelete)
    {
        uiGameObjects.erase(go);
    }

    uiMarkedForDelete.clear();
}