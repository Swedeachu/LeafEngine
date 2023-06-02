#ifndef GAME_SCENE_SYSTEM_H
#define GAME_SCENE_SYSTEM_H

#include "GameScene.h"

namespace Engine
{

	class GameSceneSystem
	{

	public:

		GameSceneSystem();

		void SetGameScene(const GameScene& scene);

		void StartGameScene();

		void RestartCurrentScene();

		void UpdateCurrentScene();

		void ExitCurrentScene();

	private:

		GameScene currentScene;

	};

} // Engine

#endif // GAME_SCENE_SYSTEM_H
