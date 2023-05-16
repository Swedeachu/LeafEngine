#include "Engine\LeafEngine.h"

#include "Engine\GameScene\GameSceneSystem.h"

#include "Game\Scenes\Sandbox.h"

namespace Entry
{
	// creates an instance of the leaf engine
	Engine::LeafEngine engine(1, 300, 300, L"Leaf Test");
}

using namespace Entry;

int main()
{
	// create an instance of the SandBox scene
	//Scenes::SandBox sandBoxScene;  
	// set the game scene to the sand box scene
	//engine.gameSceneSystem.SetGameScene(sandBoxScene);
	// run instance of the leaf engine
	return engine.Run();
}
