#ifndef GAMESCENESYSTEM_H
#define GAMESCENESYSTEM_H

#include <unordered_map>
#include <string>
#include <memory>

#include "GameScene.h"
#include "..\..\Entity\EntityList.h"

namespace GameSystem
{

	class GameSceneSystem
	{

	public:

		// Entity Management
		void AddEntityToScene(const std::string& sceneName, const Entity::Entity& entity, int zLayer);
		void AddEntityToScene(const std::string& sceneName, const Entity::Entity& entity);
		void AddEntityToCurrentScene(const Entity::Entity& entity, int zLayer);
		void AddEntityToCurrentScene(const Entity::Entity& entity);

		// Scene Management
		void AddScene(const std::string& name, GameScene* scene);
		void RemoveScene(const std::string& name);
		void SetCurrentScene(const std::string& name);
		void RestartCurrentScene();

		// Engine handles these calls
		void InitScene();
		void ExitScene();
		void UpdateScene(float deltaTime);
		void RenderScene();
		void RemoveAllScenes();

	private:

		std::unordered_map<std::string, std::unique_ptr<GameScene>> scenes; // Map of GameScenes
		std::unordered_map<std::string, Entity::EntityList> entityLists; // Map of EntityLists for each GameScene
		GameScene* currentScene = nullptr;
		std::string currentSceneName;

	};

} // GameSystem

#endif
