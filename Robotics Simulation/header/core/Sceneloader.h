#pragma once
#include "core/GameObject.h"

class Game;

class SceneLoader
{
public:
	// can't define a default constructor because of the reference member is incomplete
    explicit SceneLoader(Game& game);
    ~SceneLoader() = default;

    void LoadInitialScene();

private:
    const std::string initialSceneFilePath = "assets/initialScene.xml";
    Game& mainGame;

    
};

