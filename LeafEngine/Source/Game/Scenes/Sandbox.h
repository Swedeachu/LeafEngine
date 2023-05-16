#ifndef SANDBOX_H
#define SANDBOX_H

#include "..\..\Engine\GameScene\GameScene.h"

namespace Scenes
{

	class SandBox : public Engine::GameScene
	{

	public:
		SandBox();

		void Start() override;
		void Restart() override;
		void Update() override;
		void Exit() override;
	};

} // namespace Scenes

#endif // SANDBOX_H
