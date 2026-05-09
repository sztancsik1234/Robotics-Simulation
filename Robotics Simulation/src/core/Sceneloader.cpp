#include "core/SceneLoader.h"
#include "core/Game.h"
#include "graphics/SpriteRendererComponent.h"
#include "graphics/CircleRendererComponent.h"
#include "input/MouseFollowerComponent.h"
#include "physics/StaticBoxComponent.h"
#include "physics/BallPhysicsComponent.h"
#include "physics/BounceDetectComponent.h"
#include "graphics/IRenderer.h"
#include "core/ComponentDTOs.h"
#include "tinyxml/tinyxml2.h"

#define TRACE_LOG true

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
			object.SetAnchor(dto.anchor);

			mainGame.Logger.Log(std::format("[SceneLoader] Adding SpriteRendererComponent to {} with texture '{}'", object.ToString(), dto.texturePath), LogLevel::TRACE);
			object.EmplaceComponent<SpriteRenderComponent>(mainGame.GetCamera(),
													   *spriteIface,
													   mainGame.Logger,
													   dto);
		});

	// CircleRendererComponent (tag chosen as CircleRendererComponent for consistency)
	componentFactories.try_emplace("CircleRendererComponent",
		[this](GameObject& object, const tx2::XMLElement& /*xmlElem*/)
		{
			auto* primitiveIface = dynamic_cast<IPrimitiveRenderer*>(&mainGame.Renderer);
			if (!primitiveIface)
			{
				mainGame.Logger.Log("CircleRendererComponent: Renderer does not implement IPrimitiveRenderer", LogLevel::ERROR);
				return;
			}
			mainGame.Logger.Log(std::format("[SceneLoader] Adding CircleRendererComponent to {}", object.ToString()), LogLevel::TRACE);
			object.EmplaceComponent<CircleRendererComponent>(mainGame.GetCamera(), mainGame.Logger);
		});

	// MouseFollowerComponent
	componentFactories.try_emplace("MouseFollowerComponent",
		[this](GameObject& object, const tx2::XMLElement& /*xmlElem*/)
		{
			mainGame.Logger.Log(std::format("[SceneLoader] Adding MouseFollowerComponent to {}", object.ToString()), LogLevel::TRACE);
			object.EmplaceComponent<MouseFollowerComponent>(mainGame.GetCamera().GetViewport(), mainGame.InputService);
		});

	// StaticBoxComponent
	componentFactories.try_emplace("StaticBoxComponent",
		[this](GameObject& object, const tx2::XMLElement& /*xmlElem*/)
		{

			// TODO: how are the dimentions and positions extracted from this? looked up from the gameobject?
			// TODO: Take anchor into account when calculating the position of the box collider. Currently, the position of the gameobject is used as the center of the box, which is not correct if the anchor is not {0.5, 0.5}.
			mainGame.Logger.Log(std::format("[SceneLoader] Adding StaticBoxComponent to {}", object.ToString()), LogLevel::TRACE);
			object.EmplaceComponent<StaticBoxComponent>(mainGame.Logger, mainGame.PhysicsEngine);
		}
	);

	// BallPhysicsComponent
	componentFactories.try_emplace("BallPhysicsComponent",
		[this](GameObject& object, const tx2::XMLElement& /*xmlElem*/)
		{
			mainGame.Logger.Log(std::format("[SceneLoader] Adding BallPhysicsComponent to {}", object.ToString()), LogLevel::TRACE);
			object.EmplaceComponent<BallPhysicsComponent>(mainGame.Logger, mainGame.PhysicsEngine);
		}
	);

	// BounceDetectComponent
	componentFactories.try_emplace("BounceDetectComponent",
		[this](GameObject& object, const tx2::XMLElement& xmlElem)
		{
			ILogger& logger = mainGame.Logger;

			logger.Log(std::format("[SceneLoader] Adding BounceDetectComponent to {}", object.ToString()), LogLevel::TRACE);
			float threshold = 0.1f; // default
			// check if name is BounceDetectComponent
			if (std::string(xmlElem.Name()) != "BounceDetectComponent")
			{
				logger.Log("[Sceneloader] BounceDetectComponent cannot be constructed from a non-BounceDetectComponent element. Skipping component", LogLevel::WARNING);
				return;
			}

			if (auto* threshNode = xmlElem.FirstChildElement("tresholdAcceleration"))
			{
				if (threshNode->QueryFloatText(&threshold) != tx2::XML_SUCCESS)
				{
					logger.Log("[Sceneloader] BounceDetectComponent: invalid <tresholdAcceleration> value, using default 0.1f", LogLevel::WARNING);
				}
			}
			else
			{
				logger.Log("[Sceneloader] BounceDetectComponent: missing <tresholdAcceleration>, using default 0.1f", LogLevel::WARNING);
			}

			object.EmplaceComponent<BounceDetectComponent>(mainGame.Logger, threshold);
		}
	);
}

void SceneLoader::ParseSpriteRendererXML(const tx2::XMLElement& elem,
										 SpriteRenderComponentDTO& dto)
{
	if (auto* texNode = elem.FirstChildElement("texturePath"); texNode && texNode->GetText())
		dto.texturePath = texNode->GetText();
	else
		mainGame.Logger.Log("[Sceneloader] SpriteRendererComponent missing <texturePath>, using empty path (will load default).", LogLevel::WARNING);

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

void SceneLoader::AddComponentsFromXML(GameObject& go, const tx2::XMLElement* componentsRoot)
{
	if (!componentsRoot)
	{
		mainGame.Logger.Log("[Sceneloader] AddComponentsFromXML called with null componentsRoot", LogLevel::WARNING);
		return;
	}

	for (auto* compElem = componentsRoot->FirstChildElement();
		 compElem;
		 compElem = compElem->NextSiblingElement())
	{
		std::string tag = compElem->Name(); // tag name is the component identifier
		auto it = componentFactories.find(tag);
		if (it == componentFactories.end())
		{
			mainGame.Logger.Log("[Sceneloader] Unknown component tag: " + tag, LogLevel::WARNING);
			continue;
		}
		try
		{
			it->second(go, *compElem);
		}
		catch (const DuplicateComponentException&)
		{
			mainGame.Logger.Log(std::format("[Sceneloader] Attempted to add duplicate component {} on GameObject {}. skipping.", tag, go.ToString()), LogLevel::WARNING);
			mainGame.Logger.Log("[Sceneloader] Try implementing this after removeComponent is implemented", LogLevel::INFO);

			continue;
		}
	}
}

inline unsigned int SceneLoader::RequestGameObjectId()
{
	return nextGameObjectId++;
}

GameObject SceneLoader::CreateGameObjectFromXML(const tx2::XMLElement* xmlNode)
{
	const char* nameAttr = xmlNode->Attribute("name");
	std::string name = nameAttr ? nameAttr : "Unnamed";

	// querry id attribute. If missing, use nextGameObjectId++;
	int id = 0;
	if (!xmlNode->QueryIntAttribute("id", &id))
	{
		id = RequestGameObjectId();
	}

	Transform transform;
	auto* transformNode = xmlNode->FirstChildElement("transform");

	if (transformNode == nullptr)
		mainGame.Logger.Log(std::format("[Sceneloader] {} has no <transform> section, using default.", name), LogLevel::WARNING);
	else
		transform = ParseTransformXML(transformNode);


	GameObject go(mainGame.Logger, id, transform, name);

	if (auto* compRoot = xmlNode->FirstChildElement("components"))
		AddComponentsFromXML(go, compRoot);
	else 
		mainGame.Logger.Log(std::format("[Sceneloader] {} has no <components> section.", go.ToString()), LogLevel::WARNING);

	return go;
}

// Returns a Transform with default values if no <transform> node is found.
Transform SceneLoader::ParseTransformXML(const tx2::XMLElement* xmlNode) const
{
	if (!xmlNode)
	{
		// no transform node, return default
		mainGame.Logger.Log("[Sceneloader] Nullpointer passed to ParseTransform", LogLevel::WARNING);
		return Transform();
	}
	// Parse transform
	Transform transform;
	if (auto* posNode = xmlNode->FirstChildElement("position"))
	{
		posNode->QueryFloatAttribute("x", &transform.position.x);
		posNode->QueryFloatAttribute("y", &transform.position.y);
	}
	if (auto* rotNode = xmlNode->FirstChildElement("rotation"))
	{
		float f_rotation = 0.f;
		rotNode->QueryFloatAttribute("angle", &f_rotation);
		transform.rotation = f_rotation;
	}
	if (auto* scaleNode = xmlNode->FirstChildElement("size"))
	{
		scaleNode->QueryFloatAttribute("x", &transform.size.x);
		scaleNode->QueryFloatAttribute("y", &transform.size.y);
	}
	
	return transform;
}

static Transform MergeTransformFromXML(const tx2::XMLElement* xmlNode, Transform base)
{
	if (!xmlNode)
		return base;

	if (auto* posNode = xmlNode->FirstChildElement("position"))
	{
		if (posNode->Attribute("x"))
			posNode->QueryFloatAttribute("x", &base.position.x);
		if (posNode->Attribute("y"))
			posNode->QueryFloatAttribute("y", &base.position.y);
	}

	if (auto* rotNode = xmlNode->FirstChildElement("rotation"))
	{
		if (rotNode->Attribute("angle"))
		{
			float angle = 0.0f;
			rotNode->QueryFloatAttribute("angle", &angle);
			base.rotation = angle;
		}
	}

	if (auto* scaleNode = xmlNode->FirstChildElement("size"))
	{
		if (scaleNode->Attribute("x"))
			scaleNode->QueryFloatAttribute("x", &base.size.x);
		if (scaleNode->Attribute("y"))
			scaleNode->QueryFloatAttribute("y", &base.size.y);
	}

	return base;
}

Scene SceneLoader::LoadScene(const std::string& path)
{
	
	if constexpr (TRACE_LOG) mainGame.Logger.Log("[Sceneloader] Loading initial scene: " + path);
	Scene scene(path);

	// load document
	tx2::XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tx2::XML_SUCCESS)
	{
		mainGame.Logger.Log("[Sceneloader] Failed to load initial scene file: " + path, LogLevel::ERROR);
		return scene;
	}

	// find root
	tx2::XMLElement* root = doc.RootElement();
	if (!root || std::string(root->Name()) != "scene")
	{
		mainGame.Logger.Log("[Sceneloader] Scene file missing <scene> root.", LogLevel::ERROR);
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
				mainGame.Logger.Log("[Sceneloader] Prefab <gameObject> without valid id attribute.", LogLevel::WARNING);
				continue;
			}
			prefabMap[prefabId] = objectNode;
		}
	}
	else
	{
		mainGame.Logger.Log("Scene file missing <prefabs> section.", LogLevel::WARNING);
	}

	if constexpr (TRACE_LOG) mainGame.Logger.Log("[Sceneloader] Prefabs registered: " + std::to_string(prefabMap.size()));

	// Second pass: instantiate gameObjects section
	if (auto* gosNode = root->FirstChildElement("gameObjects"))
	{
		SecondPass(gosNode, &scene);
	}
	else
	{
		mainGame.Logger.Log("[Sceneloader] Scene file missing the whole <gameObjects> section. Scene empty", LogLevel::ERROR);
	}

	if constexpr (TRACE_LOG) {
		mainGame.Logger.Log("[Sceneloader] secondPass over...");
		scene.LogGameObjects(mainGame.Logger, true);
	}

	prefabMap.clear();
	return scene;
}

// The prefabs are already registered, now instantiate the game objects
const void SceneLoader::SecondPass(tinyxml2::XMLElement* gosNode, Scene* scene)
{
	if constexpr (TRACE_LOG) mainGame.Logger.Log("[Sceneloader] Second pass: instantiating game objects...");
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
				mainGame.Logger.Log("[Sceneloader] Prefab id not found: " + std::to_string(refId), LogLevel::WARNING);
				continue;
			}

			GameObject instance = CreateGameObjectFromXML(it->second);


			//querry transform node
			if (auto const* transformNode = childNode->FirstChildElement("transform"))
			{
				// BUG: if a new transform node here is partially written, then the omitted fields override the existing ones.
				instance.SetTransform(MergeTransformFromXML(transformNode, instance.GetTransform()));
			}

			// querry components node
			if (auto const* componentsNode = childNode->FirstChildElement("components"))
			{
				AddComponentsFromXML(instance, componentsNode);
			}
			mainGame.Logger.Log(std::format("[Sceneloader] Instantiated prefab id={} as GameObject id={}, name='{}'",
											std::to_string(refId), 
											std::to_string(instance.GetId()), 
											instance.GetName()),
								LogLevel::INFO);
			scene->AddGameObject(std::move(instance));
		}
		else if (nodeTag == "gameObject")
		{

			GameObject object = CreateGameObjectFromXML(childNode);
			scene->AddGameObject(std::move(object));
		}
		else
		{
			mainGame.Logger.Log("[Sceneloader] Unknown tag inside <gameObjects>: " + nodeTag, LogLevel::WARNING);
		}
	}
}