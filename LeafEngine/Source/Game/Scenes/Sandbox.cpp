#include "SandBox.h"
#include <iostream>

// Engine includes
#include "..\..\Engine\System\GameScene\GameSceneSystem.h"
#include "..\..\Engine\LeafMath\Vector2D.h"
#include "..\..\Engine\Entity\Entity.h"
#include "..\..\Engine\Entity\Component\Transform.h"
#include "..\..\Engine\Entity\Component\Physics.h"

// this is how we can access our leaf engine instance
#include "..\..\Engine\EngineInstance.h"

namespace Scenes
{

	void Sandbox::Init()
	{
		std::cout << "starting sandbox scene" << std::endl;

		Entity::Entity testEntity;
		Component::Transform transform(LeafMath::Vector2D(150, 150), 0, LeafMath::Vector2D(32, 32));
		testEntity.SetTransform(transform);
		Component::Physics physics(LeafMath::Vector2D(0, 0), LeafMath::Vector2D(0, 0), 0);
		testEntity.SetPhysics(physics);

		Engine::EngineInstance.GetGameSceneSystem().AddEntityToCurrentScene(testEntity);
	}

	void Sandbox::Exit()
	{
		std::cout << "exiting sandbox scene" << std::endl;
	}

	void Sandbox::Update(float deltaTime)
	{
		std::cout << "updating sandbox scene with dt " << deltaTime << std::endl;
	}

	void Sandbox::Render()
	{
		std::cout << "rendering sandbox scene" << std::endl;
	}

} // Scenes

