#pragma once

#include "core/GameObject.h"
#include <forward_list>
#include <string>
#include <util/ILogger.h>

// A simple scene holding game objects and a Box2D physics world.
class Scene
{
public:
	Scene(std::string path);

	// Move-only type.
	Scene(Scene&& other) noexcept;
	Scene& operator=(Scene&& other) noexcept;

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void Unload();

	// GameObject management.
	void AddGameObject(GameObject&& gameObject);
	void ClearGameObjects();

	// Access to the underlying container if needed.
	std::forward_list<GameObject>& GetGameObjects() { return gameObjects; }
	const std::forward_list<GameObject>& GetGameObjects() const { return gameObjects; }

	// helper for debugging
	void LogGameObjects(ILogger& logger, bool logComonents = false) const;

private:
	std::string SceneFilePath;
	std::forward_list<GameObject> gameObjects;

	void OnLoad();
	void OnUnload();
};