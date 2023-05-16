#include "Entity.h"

namespace Entity
{

	// Constructor
	Entity::Entity() : transform()
	{
		id = 0; // TO DO : make it be changed by entity system
	}

	// Destructor
	Entity::~Entity()
	{
		transform.~Transform();
	}

	// Get the transform component
	const Component::Transform& Entity::GetTransform()
	{
		return transform;
	}

	// Set the transform component
	void Entity::SetTransform(const Component::Transform& newTransform)
	{
		transform = newTransform;
	}

} // Entity
