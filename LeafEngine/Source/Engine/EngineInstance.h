#ifndef ENGINE_INSTANCE_H
#define ENGINE_INSTANCE_H

#include "LeafEngine.h"

namespace Engine
{
	// our external instance of the engine we can get from anywhere
	extern LeafEngine EngineInstance;

	// Initilizes the leaf engine with desired values
	void InitEngine(unsigned int fps, unsigned int width, unsigned int height, std::wstring title);
}

#endif // ENGINE_INSTANCE_H