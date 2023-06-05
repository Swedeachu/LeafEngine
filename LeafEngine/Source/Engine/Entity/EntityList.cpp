#include "EntityList.h"

namespace Entity
{

	// Add an Entity into an Entity List with an integer identifier which is used for its Z layer during render time
	void EntityList::AddEntity(int identifier, const Entity& entity)
	{
		EntityEntry newEntry{ identifier, &entity };

		auto insertPosition = std::lower_bound(entities.begin(), entities.end(), newEntry,
			[](const EntityEntry& entry1, const EntityEntry& entry2)
		{
			return entry1.identifier < entry2.identifier;
		});

		entities.insert(insertPosition, newEntry);
		entityCount++;
	}

	void EntityList::RemoveEntity(const Entity& entity)
	{
		auto it = std::find_if(entities.begin(), entities.end(), [&entity](const EntityEntry& entry)
		{
			return entry.entity == &entity;
		});

		if (it != entities.end())
		{
			entities.erase(it);
			delete it->entity;
			entityCount--;
		}
	}

	void EntityList::RemoveAllEntities()
	{
		for (const auto& entry : entities)
		{
			delete entry.entity;
		}
		entities.clear();
		entityCount = 0;
	}

	void EntityList::RenderEntities()
	{
		for (const auto& entry : entities)
		{
			const_cast<Entity*>(entry.entity)->EntityRender();
		}
	}

	void EntityList::UpdateEntities(float deltaTime)
	{
		for (const auto& entry : entities)
		{
			const_cast<Entity*>(entry.entity)->EntityUpdate(deltaTime);
		}
	}

} // Entity
