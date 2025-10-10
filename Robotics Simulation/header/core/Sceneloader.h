#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include "core/GameObject.h"
#include "core/ComponentDTOs.h"
#include "core/Scene.h"

// Forward declaration instead of including tinyxml2 header
namespace tinyxml2 { class XMLElement; }

class Game;

class XMLException : std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class BadTagException : std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class SceneLoader
{
public:
    explicit SceneLoader(Game& game);
    ~SceneLoader() = default;

    // Now returns a Scene built from the initial scene file.
    Scene LoadScene(const std::string& path);

private:
    Game& mainGame;
    using ComponentAddFn = std::function<void(GameObject&, const tinyxml2::XMLElement&)>;

    void RegisterDefaultComponents();
    void AddComponentsFromXML(GameObject& go, const tinyxml2::XMLElement* componentRoot);

    // Now takes a Scene& to add instantiated objects into that scene.
    void InstantiatePrefabReference(const tinyxml2::XMLElement& prefabRefElem,
                                    const std::unordered_map<int, const tinyxml2::XMLElement*>& prefabMap,
                                    Scene& scene);

    GameObject CreateGameObjectFromPrefabXML(const tinyxml2::XMLElement& prefabGoElem,
                                             int overrideId,
                                             Vector2 position);

    void ParseSpriteRendererXML(const tinyxml2::XMLElement& elem,
                                SpriteRenderComponentDTO& dto);

    std::unordered_map<std::string, ComponentAddFn> componentFactories;
};
