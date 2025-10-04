#include "core/SceneLoader.h"
#include "core/Game.h"
#include "graphics/SpriteRendererComponent.h"
#include "graphics/CircleRenderer.h"
#include "input/MouseFollowerComponent.h"
#include "graphics/IRenderer.h"
#include "core/ComponentDTOs.h"
#include "tinyxml/tinyxml2.h"

SceneLoader::SceneLoader(Game& game)
    : mainGame(game)
{
    RegisterDefaultComponents();
}

namespace tx2 = tinyxml2;

void SceneLoader::RegisterDefaultComponents()
{
    // SpriteRenderComponent
    componentFactories.emplace("SpriteRendererComponent",
        [this](GameObject& go, const tx2::XMLElement& xmlElem)
        {
            auto* spriteIface = dynamic_cast<ISpriteRenderer*>(&mainGame.Renderer);
            if (!spriteIface)
            {
                mainGame.Logger.Log("SpriteRendererComponent: Renderer does not implement ISpriteRenderer", LogLevel::ERROR);
                return;
            }

			SpriteRenderComponentDTO dto;
            ParseSpriteRendererXML(xmlElem, dto);

            go.EmplaceComponent<SpriteRenderComponent>(*spriteIface,
                                                       mainGame.Logger,
                                                       dto);
        });

    // CircleRenderer (tag chosen as CircleRendererComponent for consistency)
    componentFactories.emplace("CircleRendererComponent",
        [this](GameObject& go, const tx2::XMLElement& /*xmlElem*/)
        {
            auto* primitiveIface = dynamic_cast<IPrimitiveRenderer*>(&mainGame.Renderer);
            if (!primitiveIface)
            {
                mainGame.Logger.Log("CircleRendererComponent: Renderer does not implement IPrimitiveRenderer", LogLevel::ERROR);
                return;
            }
            go.EmplaceComponent<CircleRenderer>(*primitiveIface, mainGame.Logger);
        });

    // MouseFollowerComponent
    componentFactories.emplace("MouseFollowerComponent",
        [this](GameObject& go, const tx2::XMLElement& /*xmlElem*/)
        {
            go.EmplaceComponent<MouseFollowerComponent>(mainGame.InputService);
        });
}

void SceneLoader::ParseSpriteRendererXML(const tx2::XMLElement& elem,
                                         SpriteRenderComponentDTO& dto)
{
    if (auto* texNode = elem.FirstChildElement("texturePath"); texNode && texNode->GetText())
        dto.texturePath = texNode->GetText();
    else
        mainGame.Logger.Log("SpriteRendererComponent missing <texturePath>, using empty path (will load default).", LogLevel::WARNING);

    if (auto* sizeNode = elem.FirstChildElement("size"))
    {
        if (auto* x = sizeNode->FirstChildElement("x")) x->QueryFloatText(&dto.size.x);
        if (auto* y = sizeNode->FirstChildElement("y")) y->QueryFloatText(&dto.size.y);
    }
    else
    {
        mainGame.Logger.Log("SpriteRendererComponent missing <size>, defaulting to (0,0).", LogLevel::WARNING);
    }

    if (auto* anchorNode = elem.FirstChildElement("anchor"))
    {
        if (auto* ax = anchorNode->FirstChildElement("x")) ax->QueryFloatText(&dto.anchor.x);
        if (auto* ay = anchorNode->FirstChildElement("y")) ay->QueryFloatText(&dto.anchor.y);
    }
    // anchor optional; default already {0,0}
}

void SceneLoader::AddComponentsFromXML(GameObject& go, const tx2::XMLElement* componentRoot)
{
    if (!componentRoot) return;

    for (auto* compElem = componentRoot->FirstChildElement();
         compElem;
         compElem = compElem->NextSiblingElement())
    {
        std::string tag = compElem->Name(); // tag name is the component identifier
        auto it = componentFactories.find(tag);
        if (it == componentFactories.end())
        {
            mainGame.Logger.Log("Unknown component tag: " + tag, LogLevel::WARNING);
            continue;
        }
        it->second(go, *compElem);
    }
}

GameObject SceneLoader::CreateGameObjectFromPrefabXML(const tx2::XMLElement& prefabGoElem,
                                                      int overrideId,
                                                      Vector2 position)
{
    int prefabId = 0;
    prefabGoElem.QueryIntAttribute("id", &prefabId);
    const char* nameAttr = prefabGoElem.Attribute("name");
    std::string name = nameAttr ? nameAttr : "Unnamed";

    // If overrideId != 0 use it; else use prefab id.
    int finalId = overrideId != 0 ? overrideId : prefabId;

    GameObject go(mainGame.Logger, finalId, position, name);

    if (auto* compRoot = prefabGoElem.FirstChildElement("component"))
        AddComponentsFromXML(go, compRoot);

    return go;
}

void SceneLoader::InstantiatePrefabReference(const tinyxml2::XMLElement& prefabRefElem,
                                             const std::unordered_map<int, const tinyxml2::XMLElement*>& prefabMap)
{
    int refId = 0;
    prefabRefElem.QueryIntAttribute("id", &refId);

    float x = 0.f, y = 0.f;
    prefabRefElem.QueryFloatAttribute("x", &x);
    prefabRefElem.QueryFloatAttribute("y", &y);

    auto it = prefabMap.find(refId);
    if (it == prefabMap.end())
    {
        mainGame.Logger.Log("Prefab id not found: " + std::to_string(refId), LogLevel::WARNING);
        return;
    }

    GameObject instance = CreateGameObjectFromPrefabXML(*it->second, /*overrideId*/ 0, Vector2{x, y});
    mainGame.addGameObject(std::move(instance));
}

void SceneLoader::LoadInitialScene()
{
    namespace tx2 = tx2;
    tx2::XMLDocument doc;
    if (doc.LoadFile(INTIAL_SCENE_PATH.c_str()) != tx2::XML_SUCCESS)
    {
        mainGame.Logger.Log("Failed to load initial scene file: " + INTIAL_SCENE_PATH, LogLevel::ERROR);
        return;
    }

    tx2::XMLElement* root = doc.RootElement();
    if (!root || std::string(root->Name()) != "scene")
    {
        mainGame.Logger.Log("Scene file missing <scene> root.", LogLevel::ERROR);
        return;
    }

    // First pass: gather prefab definitions
    std::unordered_map<int, const tinyxml2::XMLElement*> prefabMap;
    if (auto* prefabsNode = root->FirstChildElement("prefabs"))
    {
        for (auto* goElem = prefabsNode->FirstChildElement("gameObject");
             goElem;
             goElem = goElem->NextSiblingElement("gameObject"))
        {
            int id = 0;
            goElem->QueryIntAttribute("id", &id);
            if (id == 0)
            {
                mainGame.Logger.Log("Prefab <gameObject> without valid id attribute.", LogLevel::WARNING);
                continue;
            }
            prefabMap[id] = goElem;
        }
    }

    // Second pass: instantiate gameObjects section
    if (auto* gosNode = root->FirstChildElement("gameObjects"))
    {
        for (auto* child = gosNode->FirstChildElement();
             child;
             child = child->NextSiblingElement())
        {
            std::string tag = child->Name();
            if (tag == "prefab")
            {
                InstantiatePrefabReference(*child, prefabMap);
            }
            else if (tag == "gameObject")
            {
                // Direct gameObject definition (not a prefab reference)
                int id = 0; child->QueryIntAttribute("id", &id);
                float x = 0.f, y = 0.f;
                child->QueryFloatAttribute("x", &x);
                child->QueryFloatAttribute("y", &y);

                GameObject go = CreateGameObjectFromPrefabXML(*child, id, Vector2{x, y});
                mainGame.addGameObject(std::move(go));
            }
            else
            {
                mainGame.Logger.Log("Unknown tag inside <gameObjects>: " + tag, LogLevel::WARNING);
            }
        }
    }
}