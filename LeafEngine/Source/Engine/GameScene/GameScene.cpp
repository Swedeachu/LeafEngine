#include "..\stdafx.h"
#include "GameScene.h"
#include "..\..\Entry.h"

using namespace Entry;

namespace Engine
{

	// construct a game scene with a named indentifier
	GameScene::GameScene(const std::string& name) : gameSceneName(name) {}

	// the initilizing code that gets called for the game scene
	void GameScene::Start()
	{
		std::cout << gameSceneName << " Start | Frame: " << engine.GetTotalFrames() << std::endl;
	}

	// the code that gets called when the game scene restarts
	void GameScene::Restart()
	{
		std::cout << gameSceneName << " Restart | Frame: " << engine.GetTotalFrames() << std::endl;
	}

	// the code that runs each frame update in the game scene
	void GameScene::Update()
	{
		std::cout << gameSceneName << " Update | Frame: " << engine.GetTotalFrames() << std::endl;
	}

	// the code that runs when the game scene is closed
	void GameScene::Exit()
	{
		std::cout << gameSceneName << " Exit | Frame: " << engine.GetTotalFrames() << std::endl;
	}

} // Engine