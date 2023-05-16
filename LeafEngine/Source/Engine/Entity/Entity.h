#include "Component/Transform.h"

namespace Entity
{
	class Entity
	{

	private:
		int id;
		Component::Transform transform;

	public:

		// Constructor
		Entity();

		// Destructor
		~Entity();

		// Get the transform component
		const Component::Transform& GetTransform();

		// Set the transform component
		void SetTransform(const Component::Transform& newTransform);

	};

} // Entity
