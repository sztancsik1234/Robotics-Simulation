#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include "core/GameObject.h"
#include "core/ComponentDTOs.h"

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

    void LoadInitialScene();

private:
    const std::string INTIAL_SCENE_PATH = "assets/initialScene.xml";
    Game& mainGame;
    using ComponentAddFn = std::function<void(GameObject&, const tinyxml2::XMLElement&)>;

    void RegisterDefaultComponents();
    void AddComponentsFromXML(GameObject& go, const tinyxml2::XMLElement* componentRoot);
    void InstantiatePrefabReference(const tinyxml2::XMLElement& prefabRefElem,
                                    const std::unordered_map<int, const tinyxml2::XMLElement*>& prefabMap);
    GameObject CreateGameObjectFromPrefabXML(const tinyxml2::XMLElement& prefabGoElem,
                                             int overrideId,
                                             Vector2 position);

    void ParseSpriteRendererXML(const tinyxml2::XMLElement& elem,
                                SpriteRenderComponentDTO& dto);

    std::unordered_map<std::string, ComponentAddFn> componentFactories;
};
