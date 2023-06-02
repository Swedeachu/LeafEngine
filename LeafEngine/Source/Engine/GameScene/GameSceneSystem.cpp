#include "../stdafx.h"

#include "GameSceneSystem.h"

namespace Engine
{

	GameSceneSystem::GameSceneSystem() : currentScene(nullptr) {}

	void GameSceneSystem::SetGameScene(const GameScene& scene)
	{
		currentScene = scene;
		currentScene.Start();
	}

	void GameSceneSystem::StartGameScene()
	{
		currentScene.Start();
	}

	void GameSceneSystem::RestartCurrentScene()
	{
		currentScene.Restart();
	}

	void GameSceneSystem::UpdateCurrentScene()
	{
		currentScene.Update();
	}

	void GameSceneSystem::ExitCurrentScene()
	{
		currentScene.Exit();
	}

} // Engine
