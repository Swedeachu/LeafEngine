#include "Entity.h"

namespace Entity
{

	// Constructor
	Entity::Entity()
	{
		hasTransform = false;
		hasPhysics = false;
		hasMesh = false;
	}

	Entity::Entity(const Transform& newTransform, const Physics& newPhysics, const Mesh& newMesh) : transform(newTransform), physics(newPhysics), mesh(newMesh)
	{
		hasTransform = true;
		hasPhysics = true;
		hasMesh = true;
	}

	// Destructor
	Entity::~Entity()
	{}

	// Get the transform component
	const Transform& Entity::GetTransform()
	{
		return transform;
	}

	// Set the transform component
	void Entity::SetTransform(const Transform& newTransform)
	{
		hasTransform = true;
		transform = newTransform;
	}

	// Get the physics component
	const Physics& Entity::GetPhysics()
	{
		return physics;
	}

	// Set the physics component
	void Entity::SetPhysics(const Physics& newPhysics)
	{
		hasPhysics = true;
		physics = newPhysics;
	}

	// Get the mesh component
	const Mesh& Entity::GetMesh()
	{
		return mesh;
	}

	// Set the mesh component
	void Entity::SetMesh(const Mesh& newMesh)
	{
		hasMesh = true;
		mesh = newMesh;
	}

	// Render the entity
	void Entity::EntityRender()
	{

	}

	// Update the entity
	void Entity::EntityUpdate(float deltaTime)
	{
		if (hasPhysics && hasTransform)
		{
			physics.PhysicsUpdate(transform, deltaTime);
		}
	}

} // Entity
