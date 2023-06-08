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

		// make a pointer to the test entity to dynamically allocate it
		Entity::Entity* testEntity = new Entity::Entity();
		Component::Transform transform(LeafMath::Vector2D(5, 5), 0, LeafMath::Vector2D(1, 1));
		testEntity->SetTransform(transform);
		Component::Physics physics(LeafMath::Vector2D(0, 0), LeafMath::Vector2D(0, 0), 0);
		testEntity->SetPhysics(physics);

		Engine::EngineInstance.GetGameSceneSystem().AddEntityToCurrentScene(*testEntity);

		// Modify the camera's position and zoom
		Engine::EngineInstance.GetCamera().SetPosition(0, 0);
		// Engine::EngineInstance.GetCamera().SetZoom(2.0f);
	}

	void Sandbox::Exit()
	{
		std::cout << "exiting sandbox scene" << std::endl;
	}

	void Sandbox::Update(float deltaTime)
	{
		if (Engine::EngineInstance.InputKeyDown('W'))
		{
			std::cout << "pressed w" << std::endl;
		}
	}

	void Sandbox::Render()
	{
		// std::cout << "rendering sandbox scene" << std::endl;
	}

} // Scenes

