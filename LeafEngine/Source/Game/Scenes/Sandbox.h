#ifndef SANDBOX_H
#define SANDBOX_H

#include "..\..\Engine\System\GameScene\GameScene.h"

namespace Scenes
{

	class Sandbox : public GameSystem::GameScene
	{

	public:

		void Init() override;
		void Exit() override;
		void Update(float deltaTime) override;
		void Render() override;

	};

} // Scenes

#endif // SANDBOX_H

