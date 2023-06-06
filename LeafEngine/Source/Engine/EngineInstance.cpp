#include "EngineInstance.h"

namespace Engine
{

	// External global instance of the engine.
	// It is constructed with default values, but not started.
	// Call this files InitEngine function with parameters to create it with desired values!
	// Default values are 60 FPS, 300 x 300 window, and title "Leaf Engine Game"
	LeafEngine EngineInstance; 

	// Initilizes the global engine instance with an fps, window width and height, and a window title
	void InitEngine(unsigned int fps, unsigned int width, unsigned int height, std::wstring title)
	{
		EngineInstance.SetFrameRate(fps);
		EngineInstance.SetWindowSize(width, height);
		EngineInstance.SetWindowTitle(title);
	}

} // Engine