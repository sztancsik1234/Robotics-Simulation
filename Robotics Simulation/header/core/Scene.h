#pragma once

#include <forward_list>
#include <utility>
#include "core/GameObject.h"

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
	//Q: Why delete copy constructor and copy assignment operator?

    void Unload();

    // GameObject management.
    void addGameObject(GameObject&& gameObject);
    void clearGameObjects();

    // Access to the underlying container if needed.
    std::forward_list<GameObject>& getGameObjects() { return gameObjects; }
    const std::forward_list<GameObject>& getGameObjects() const { return gameObjects; }

	// helper for debugging
	void logGameObjects(ILogger& logger) const;

private:
    std::string SceneFilePath;
    std::forward_list<GameObject> gameObjects;

    void OnLoad();
    void OnUnload();
};