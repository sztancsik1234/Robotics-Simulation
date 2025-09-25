#include "core/SceneLoader.h"
#include "core/Game.h"
#include "tinyxml/tinyxml2.h"

SceneLoader::SceneLoader(Game& game)
    : mainGame(game)
{}

void SceneLoader::LoadInitialScene()
{
	namespace TX2 = tinyxml2;
	// load the XMLDocument
	TX2::XMLDocument doc;
	TX2::XMLError eResult = doc.LoadFile(initialSceneFilePath.c_str());
	if (eResult != TX2::XML_SUCCESS) {
		mainGame.Logger.Log("Failed to load initial scene file: " + initialSceneFilePath, LogLevel::ERROR);
		return;
	}

	// get the root element
	TX2::XMLElement* root = doc.RootElement();
	// get the prefabs tag

	// iterate over the gameObject-s
		
		// parse the gameobject
		
		// save a pointer to an std::map based on id

	// iterate over the gameobjects tag.

	// if a tag is prefab, instantiate from the prefab map

	// if it's a new gameobject, parse it then add it to the game.
}