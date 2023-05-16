#ifndef INTERNAL_SYSTEM_H
#define INTERNAL_SYSTEM_H

#include <string>
#include <functional>

namespace Engine
{

	class InternalSystem
	{

	private:

		std::function<void()> initFunc;
		std::function<void()> updateFunc;
		std::function<void()> exitFunc;

	public:

		InternalSystem(const std::function<void()>& init, const std::function<void()>& update, const std::function<void()>& exit, std::string systemName);

		// Copy constructor
		InternalSystem(const InternalSystem& other);

		// Assignment operator
		InternalSystem& operator=(const InternalSystem& other);

		void Init() const;
		void Update() const;
		void Exit() const;

		std::string systemName;

	};

} // Engine

#endif // INTERNAL_SYSTEM_H
