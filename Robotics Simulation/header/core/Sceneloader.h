#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
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
    inline unsigned int RequestGameObjectId();

    // Single factory that creates a GameObject from a <gameObject> XML element.
    GameObject CreateGameObjectFromXML(const tinyxml2::XMLElement* xmlNode,
                                       std::optional<int> overrideId = std::nullopt,
                                       std::optional<Transform> overrideTransform = std::nullopt);
	inline Transform ParseTransformXML(const tinyxml2::XMLElement* xmlNode) const;

    void ParseSpriteRendererXML(const tinyxml2::XMLElement& elem,
                                SpriteRenderComponentDTO& dto);
    const void SecondPass(tinyxml2::XMLElement* gosNode, Scene* scene);

	std::unordered_map<std::string, ComponentAddFn> componentFactories; //TODO: stored functions should construct a component and return it. The caller should be resposible for adding it to the gameobject.
    std::unordered_map<int, const tinyxml2::XMLElement*> prefabMap;

    unsigned int nextGameObjectId = 1;
};
