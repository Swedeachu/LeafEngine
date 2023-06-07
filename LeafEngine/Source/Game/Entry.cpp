// engine includes
#include "..\Engine\LeafEngine.h"
#include "..\Engine\System\GameScene\GameScene.h"
#include "..\Engine\System\GameScene\GameSceneSystem.h"

// engine includes
#include "..\Engine\EngineInstance.h"

// game includes
#include "..\Game\Scenes\Sandbox.h"

int main()
{
	// init the engine so the external Engine::EngineInstane is set up
	Engine::InitEngine(60, 1500, 700, L"Leaf Test");

	// create an instance of the SandBox scene
	Scenes::Sandbox sandbox;
	Engine::EngineInstance.GetGameSceneSystem().AddScene("sandbox", &sandbox); // add it to the scene list
	Engine::EngineInstance.GetGameSceneSystem().SetCurrentScene("sandbox", false, true); // set it to the active scene

	// run our instance of the leaf engine
	return Engine::EngineInstance.Run();
}
