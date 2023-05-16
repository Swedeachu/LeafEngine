#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "../stdafx.h"

namespace Engine
{

	class GameScene
	{

	public:

		GameScene(const std::string& name);

		virtual void Start();
		virtual void Restart();
		virtual void Update();
		virtual void Exit();

	private:

		std::string gameSceneName;

	};

} // Engine

#endif // GAME_SCENE_H
