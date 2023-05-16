#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include <vector>
#include "InternalSystem.h"

namespace Engine
{

	class GameSystem
	{

	private:

		static std::vector<InternalSystem> systems;

	public:

		static void InitSystems();
		static void UpdateSystems();
		static void ExitSystems();

		static void AddSystem(const InternalSystem& system);
		static void RemoveSystem(const InternalSystem& system);

	};

} // Engine

#endif // GAME_SYSTEM_H