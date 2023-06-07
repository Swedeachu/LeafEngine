#include "GameSceneSystem.h"

namespace GameSystem
{

#pragma region Adding Entities to scene

	// Add an entity to the specified scene with an optional z-layer parameter (default zLayer is 1)
	void GameSceneSystem::AddEntityToScene(const std::string& sceneName, const Entity::Entity& entity, int zLayer)
	{
		if (scenes.find(sceneName) != scenes.end())
		{
			entityLists[sceneName].AddEntity(zLayer, entity);
		}
	}

	// Add an entity to the current scene with an optional z-layer parameter (default zLayer is 1)
	void GameSceneSystem::AddEntityToCurrentScene(const Entity::Entity& entity, int zLayer)
	{
		AddEntityToScene(currentSceneName, entity, zLayer);
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

	// Set the current scene by name
	// exitOldScene determines if the old scenes exit function should be called on switching
	// restartNewScene determines if the new scenes init function should be called on switching
	// these booleans determine how you want your scene swapping to behave, if it is just a literal swap or has full scene memory resets involved
	void GameSceneSystem::SetCurrentScene(const std::string& name, bool exitOldScene, bool restartNewScene)
	{
		auto it = scenes.find(name);
		if (it != scenes.end())
		{
			if (currentScene != nullptr)
			{
				if (exitOldScene)
				{
					currentScene->Exit();
				}
			}
			currentSceneName = name;
			currentScene = it->second.get();
			if (restartNewScene)
			{
				currentScene->Init();
			}
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

	// Calls the curernt scenes exit function
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
			entityLists[currentSceneName].UpdateEntities(deltaTime);
			currentScene->Update(deltaTime);
		}
	}

	// Render's all Entities in a scene and calls a scene's render function
	void GameSceneSystem::RenderScene()
	{
		if (currentScene != nullptr)
		{
			entityLists[currentSceneName].RenderEntities();
			currentScene->Render();
		}
	}

	// callExitFunction determines if this calls the current scene's exit function, then instantly calls it's init function
	// Otherwise this only calls the init function again after clearing the entity list
	void GameSceneSystem::RestartCurrentScene(bool callExitFunction)
	{
		if (currentScene != nullptr)
		{
			if (callExitFunction)
			{
				currentScene->Exit();
			}
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