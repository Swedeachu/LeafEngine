#include "InternalSystem.h"

namespace Engine
{

	InternalSystem::InternalSystem(const std::function<void()>& init, const std::function<void()>& update, const std::function<void()>& exit, std::string systemName)
		: initFunc(init), updateFunc(update), exitFunc(exit), systemName(systemName)
	{}

	// Copy constructor
	InternalSystem::InternalSystem(const InternalSystem& other)
		: initFunc(other.initFunc), updateFunc(other.updateFunc), exitFunc(other.exitFunc), systemName(other.systemName)
	{}

	// Assignment operator
	InternalSystem& InternalSystem::operator=(const InternalSystem& other)
	{
		if (this != &other)
		{
			initFunc = other.initFunc;
			updateFunc = other.updateFunc;
			exitFunc = other.exitFunc;
			systemName = other.systemName;
		}
		return *this;
	}

	void InternalSystem::Init() const
	{
		if (initFunc)
		{
			initFunc();
		}
	}

	void InternalSystem::Update() const
	{
		if (updateFunc)
		{
			updateFunc();
		}
	}

	void InternalSystem::Exit() const
	{
		if (exitFunc)
		{
			exitFunc();
		}
	}

} // Engine
