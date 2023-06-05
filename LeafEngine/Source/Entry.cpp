// engine includes
#include "Engine\LeafEngine.h"
#include "Engine\System\GameScene\GameScene.h"
#include "Engine\System\GameScene\GameSceneSystem.h"

// game includes
#include "Game\Scenes\Sandbox.h"

#include "Entry.h"
namespace Entry
{
	// create the engine
	Engine::LeafEngine engine(60, 300, 300, L"Leaf Test");
}
using namespace Entry;

int main()
{
	// create an instance of the SandBox scene 
	Scenes::Sandbox sandbox;
	engine.GetGameSceneSystem().AddScene("sandbox", &sandbox); // add it to the scene list
	engine.GetGameSceneSystem().SetCurrentScene("sandbox"); // set it to the active scene

	// run instance of the leaf engine
	return engine.Run();
}
