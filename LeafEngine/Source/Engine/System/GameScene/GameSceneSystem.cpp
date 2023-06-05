#include "GameSceneSystem.h"

namespace GameSystem
{

#pragma region Adding Entities to scene

	// Add an entity to the specified scene with an optional z-layer parameter
	void GameSceneSystem::AddEntityToScene(const std::string& sceneName, const Entity::Entity& entity, int zLayer)
	{
		if (scenes.find(sceneName) != scenes.end())
		{
			entityLists[sceneName].AddEntity(zLayer, entity);
		}
	}

	// Add an entity to the specified scene with a default z-layer value of 1
	void GameSceneSystem::AddEntityToScene(const std::string& sceneName, const Entity::Entity& entity)
	{
		if (scenes.find(sceneName) != scenes.end())
		{
			entityLists[sceneName].AddEntity(1, entity); // Default zLayer value of 1
		}
	}

	// Add an entity to the current scene with an optional z-layer parameter
	void GameSceneSystem::AddEntityToCurrentScene(const Entity::Entity& entity, int zLayer)
	{
		AddEntityToScene(currentSceneName, entity, zLayer);
	}

	// Add an entity to the current scene with a default z-layer of 1
	void GameSceneSystem::AddEntityToCurrentScene(const Entity::Entity& entity)
	{
		AddEntityToScene(currentSceneName, entity, 1); // Default zLayer value of 1
	}

#pragma endregion End of adding entities to scene

	// Add a scene with a name to the GameSceneSystem
	void GameSceneSystem::AddScene(const std::string& name, GameScene* scene)
	{
		scenes[name] = std::unique_ptr<GameScene>(scene);
		entityLists[name] = Entity::EntityList(); // Create a new EntityList for the GameScene
	}

	// Remove a scene by name
	void GameSceneSystem::RemoveScene(const std::string& name)
	{
		entityLists[name].RemoveAllEntities();
		entityLists.erase(name);
		scenes.erase(name);
	}

	// Set the current scene by name, this calls the old current scene's exit function and then the new current scene's init function immediately
	void GameSceneSystem::SetCurrentScene(const std::string& name)
	{
		auto it = scenes.find(name);
		if (it != scenes.end())
		{
			if (currentScene != nullptr)
			{
				currentScene->Exit();
			}
			currentSceneName = name;
			currentScene = it->second.get();
			currentScene->Init();
		}
	}

	// Starts the scene and calls the current scenes init function
	void GameSceneSystem::InitScene()
	{
		if (currentScene != nullptr)
		{
			currentScene->Init();
		}
	}

	// Free's all Entities in a scene and calls the curernt scenes exit function
	void GameSceneSystem::ExitScene()
	{
		if (currentScene != nullptr)
		{
			currentScene->Exit();
		}
	}

	// Update's all Entities in a scene and calls the current scenes update function
	void GameSceneSystem::UpdateScene(float deltaTime)
	{
		if (currentScene != nullptr)
		{
			Entity::EntityList& entityList = entityLists[currentSceneName];
			entityList.UpdateEntities(deltaTime);
			currentScene->Update(deltaTime);
		}
	}

	// Render's all Entities in a scene and calls a scene's render function
	void GameSceneSystem::RenderScene()
	{
		if (currentScene != nullptr)
		{
			Entity::EntityList& entityList = entityLists[currentSceneName];
			entityList.RenderEntities();
			currentScene->Render();
		}
	}

	// Calls the current scene's exit function, then instantly calls it's init function
	void GameSceneSystem::RestartCurrentScene()
	{
		if (currentScene != nullptr)
		{
			currentScene->Exit();
			currentScene->Init();
		}
	}

	// Removes all scenes effectively freeing all entity lists
	void GameSceneSystem::RemoveAllScenes()
	{
		for (auto& entry : scenes)
		{
			entityLists[entry.first].RemoveAllEntities();
		}
		// then clear
		scenes.clear();
		entityLists.clear();
		currentScene = nullptr;
		currentSceneName.clear();
	}


} // GameSystem