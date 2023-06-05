#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <map>
#include <vector>

#include "Entity.h"

namespace Entity
{

	class EntityList
	{

	public:

		void AddEntity(int identifier, const Entity& entity);

		void RemoveEntity(const Entity& entity);

		void RemoveAllEntities();

		void RenderEntities();

		void UpdateEntities(float deltaTime);

	private:

		struct EntityEntry
		{
			int identifier;
			const Entity* entity;
		};

		std::vector<EntityEntry> entities;

		int entityCount = 0;

	};

} // Entity

#endif
