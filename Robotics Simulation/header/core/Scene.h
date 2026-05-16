#pragma once

#include "core/GameObject.h"
#include <list>
#include <string>
#include <util/ILogger.h>

// A simple scene holding game objects and a Box2D physics world.
class Scene
{
public:
	explicit Scene(std::string path);

	// Move-only type.
	Scene(Scene&& other) noexcept;
	Scene& operator=(Scene&& other) noexcept;

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void Unload();

	// GameObject management.
	void MoveGameObject(GameObject&& gameObject);
	GameObject* AddGameObject(const GameObject& gameObject);
	void AddUiGameObject(GameObject&& uiGameObject);
	void ClearGameObjects();

	

	// Access to the underlying container if needed.
	std::list<GameObject>& GetGameObjects() { return gameObjects; }
	const std::list<GameObject>& GetGameObjects() const { return gameObjects; }

	// Access to the underlying container if needed.
	std::list<GameObject>& GetUiGameObjects() { return uiGameObjects; }
	const std::list<GameObject>& GetUiGameObjects() const { return uiGameObjects; }

	// helper for debugging
	void LogGameObjects(ILogger& logger, bool logComonents = false) const;

private:
	std::string SceneFilePath;
	std::list<GameObject> gameObjects;
	std::list<GameObject> uiGameObjects;

	void OnLoad();
	void OnUnload();
};