#include "GameSystem.h"

namespace Engine
{

	std::vector<InternalSystem> GameSystem::systems;

	void GameSystem::InitSystems()
	{
		for (auto& system : systems)
		{
			system.Init();
		}
	}

	void GameSystem::UpdateSystems()
	{
		for (auto& system : systems)
		{
			system.Update();
		}
	}

	void GameSystem::ExitSystems()
	{
		for (auto& system : systems)
		{
			system.Exit();
		}
	}

	void GameSystem::AddSystem(const InternalSystem& system)
	{
		systems.push_back(system);
	}

	void GameSystem::RemoveSystem(const InternalSystem& system)
	{
		auto it = systems.begin();
		while (it != systems.end())
		{
			if (it->systemName == system.systemName)
			{
				it = systems.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

} // Engine
