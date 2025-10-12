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
	componentFactories.try_emplace("SpriteRendererComponent",
		[this](GameObject& object, const tx2::XMLElement& xmlElem)
		{
			auto* spriteIface = dynamic_cast<ISpriteRenderer*>(&mainGame.Renderer);
			if (!spriteIface)
			{
				mainGame.Logger.Log("SpriteRendererComponent: Renderer does not implement ISpriteRenderer", LogLevel::ERROR);
				return;
			}

			SpriteRenderComponentDTO dto;
			ParseSpriteRendererXML(xmlElem, dto);

			object.EmplaceComponent<SpriteRenderComponent>(*spriteIface,
													   mainGame.Logger,
													   dto);
		});

	// CircleRenderer (tag chosen as CircleRendererComponent for consistency)
	componentFactories.try_emplace("CircleRendererComponent",
		[this](GameObject& object, const tx2::XMLElement& /*xmlElem*/)
		{
			auto* primitiveIface = dynamic_cast<IPrimitiveRenderer*>(&mainGame.Renderer);
			if (!primitiveIface)
			{
				mainGame.Logger.Log("CircleRendererComponent: Renderer does not implement IPrimitiveRenderer", LogLevel::ERROR);
				return;
			}
			object.EmplaceComponent<CircleRenderer>(*primitiveIface, mainGame.Logger);
		});

	// MouseFollowerComponent
	componentFactories.try_emplace("MouseFollowerComponent",
		[this](GameObject& object, const tx2::XMLElement& /*xmlElem*/)
		{
			object.EmplaceComponent<MouseFollowerComponent>(mainGame.InputService);
		});

	// TODO: physics component
}

void SceneLoader::ParseSpriteRendererXML(const tx2::XMLElement& elem,
										 SpriteRenderComponentDTO& dto)
{
	if (auto* texNode = elem.FirstChildElement("texturePath"); texNode && texNode->GetText())
		dto.texturePath = texNode->GetText();
	else
		mainGame.Logger.Log("SpriteRendererComponent missing <texturePath>, using empty path (will load default).", LogLevel::WARNING);

	if (auto* anchorNode = elem.FirstChildElement("anchor"))
	{

		float ax = dto.anchor.x;
		float ay = dto.anchor.y;

		auto resX = anchorNode->QueryFloatAttribute("x", &ax);
		auto resY = anchorNode->QueryFloatAttribute("y", &ay);

		// Backward compatibility: nested <x> / <y> nodes
		if (resX != tx2::XML_SUCCESS)
		{
			if (auto* axElem = anchorNode->FirstChildElement("x"))
				axElem->QueryFloatText(&ax);
		}
		if (resY != tx2::XML_SUCCESS)
		{
			if (auto* ayElem = anchorNode->FirstChildElement("y"))
				ayElem->QueryFloatText(&ay);
		}

		dto.anchor = { ax, ay };
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

inline unsigned int SceneLoader::RequestGameObjectId()
{
	return nextGameObjectId++;
}

GameObject SceneLoader::CreateGameObjectFromXML(const tx2::XMLElement& xmlNode)
{
	const char* nameAttr = xmlNode.Attribute("name");
	std::string name = nameAttr ? nameAttr : "Unnamed";

	// querry id attribute. If missing, use nextGameObjectId++;
	int id;
	if (!xmlNode.QueryIntAttribute("id", &id))
	{
		id = RequestGameObjectId();
	}

	auto transformNode = xmlNode.FirstChildElement("transform");

	Transform transform = ParseTransformXML(xmlNode);

	GameObject go(mainGame.Logger, id, transform, name);

	if (auto* compRoot = xmlNode.FirstChildElement("component"))
		AddComponentsFromXML(go, compRoot);

	return go;
}

Transform SceneLoader::ParseTransformXML(const tx2::XMLElement& xmlNode)
{
	// Parse transform
	Transform transform;
	if (auto* transNode = xmlNode.FirstChildElement("transform"))
	{
		if (auto* posNode = transNode->FirstChildElement("position"))
		{
			posNode->QueryFloatAttribute("x", &transform.position.x);
			posNode->QueryFloatAttribute("y", &transform.position.y);
		}
		if (auto* rotNode = transNode->FirstChildElement("rotation"))
		{
			float f_rotation = 0.f;
			rotNode->QueryFloatAttribute("angle", &f_rotation);
			transform.rotation = f_rotation;
		}
		if (auto* scaleNode = transNode->FirstChildElement("size"))
		{
			scaleNode->QueryFloatAttribute("x", &transform.size.x);
			scaleNode->QueryFloatAttribute("y", &transform.size.y);
		}
	}
	return transform;
}

Scene SceneLoader::LoadScene(const std::string& path)
{
	Scene scene(path);

	// load document
	tx2::XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tx2::XML_SUCCESS)
	{
		mainGame.Logger.Log("Failed to load initial scene file: " + path, LogLevel::ERROR);
		return scene;
	}

	// find root
	tx2::XMLElement* root = doc.RootElement();
	if (!root || std::string(root->Name()) != "scene")
	{
		mainGame.Logger.Log("Scene file missing <scene> root.", LogLevel::ERROR);
		return scene;
	}

	// First pass: gather prefab definitions
	if (auto* prefabsNode = root->FirstChildElement("prefabs"))
	{
		for (auto* objectNode = prefabsNode->FirstChildElement("gameObject");
			 objectNode;
			 objectNode = objectNode->NextSiblingElement("gameObject"))
		{
			int prefabId = 0;
			objectNode->QueryIntAttribute("id", &prefabId);
			if (prefabId == 0)
			{
				mainGame.Logger.Log("Prefab <gameObject> without valid id attribute.", LogLevel::WARNING);
				continue;
			}
			prefabMap[prefabId] = objectNode;
		}
	}
	else
	{
		mainGame.Logger.Log("Scene file missing <prefabs> section.", LogLevel::WARNING);
	}

	// Second pass: instantiate gameObjects section
	if (auto* gosNode = root->FirstChildElement("gameObjects"))
	{
		SecondPass(gosNode, &scene);
	}
	else
	{
		mainGame.Logger.Log("Scene file missing the whole <gameObjects> section. Scene empty", LogLevel::ERROR);
	}

	return scene;
}

// The prefabs are already registered, now instantiate the game objects
const void SceneLoader::SecondPass(tinyxml2::XMLElement* gosNode, Scene* scene)
{
	// Iterate over children, which can be <prefab> or <gameObject>
	for (auto* childNode = gosNode->FirstChildElement();
		childNode;
		childNode = childNode->NextSiblingElement())
	{
		// Check if prefab or direct gameObject
		std::string nodeTag = childNode->Name();
		if (nodeTag == "prefab")
		{
			int refId = 0;
			childNode->QueryIntAttribute("id", &refId);

			auto it = prefabMap.find(refId);
			if (it == prefabMap.end())
			{
				mainGame.Logger.Log("Prefab id not found: " + std::to_string(refId), LogLevel::WARNING);
				continue;
			}

			GameObject instance = CreateGameObjectFromXML(*it->second);


			//querry transform node
			if (auto const* transformNode = childNode->FirstChildElement("transform"))
			{
				instance.SetTransform(ParseTransformXML(*transformNode));
			}

			scene->addGameObject(std::move(instance));
		}
		else if (nodeTag == "gameObject")
		{

			GameObject object = CreateGameObjectFromXML(*childNode);
			scene->addGameObject(std::move(object));
		}
		else
		{
			mainGame.Logger.Log("Unknown tag inside <gameObjects>: " + nodeTag, LogLevel::WARNING);
		}
	}
}

/*
#class:GameObject:350-3000 has recently been adjusted to use #class:sf\:\:Transform:1567-10143 instead of a simple #class:sf\:\:Vector2:1429-8400 . Consequently, so did it's constructor.

Adjust 
*/


/*
GitHub Copilot

Short answer : you don’t move tinyxml2::XMLElement; you move GameObject.

Details:
-std::move is just a cast. The actual move depends on the type.
- tinyxml2::XMLElement is a non - owning, non - movable node type tied to its XMLDocument.You typically pass it as const XMLElement & or const XMLElement * .“Moving” an XMLElement isn’t supported; at best you copy / move a pointer, which is a trivial pointer assignment.
- GameObject is movable(GameObject(GameObject&&) noexcept).Moving it is O(1) :
	-id : copied trivially
	- name : std::string move(pointer / size swap)
	- transform : small POD - like copy / move
	- componentList : std::forward_list<std::unique_ptr<Component>> move steals the head pointer; no deep copy, no allocations

	Which is “faster” ? Copying / moving an XMLElement pointer is cheaper than moving a GameObject object, but in practice both are tiny constant - time operations.In your code, you already do the right thing :
-Prefabs map stores const tinyxml2::XMLElement * (cheap pointer copies)
- Scene stores GameObject by value and moves them into the container(cheap O(1) moves)

Note : ensure the XMLDocument outlives any stored XMLElement * in your prefabMap.
*/