#ifndef ENTITY_H
#define ENTITY_H

#include "Component/Transform.h"
#include "Component/Physics.h"
#include "../Graphics/MeshLibrary.h"

using namespace Component;

namespace Entity
{

	class Entity
	{

	private:

		// components
		Transform transform;
		Physics physics;

		Graphics::MeshType meshType;

		bool hasTransform;
		bool hasPhysics;

	public:

		// Constructor
		Entity();

		Entity(const Transform& newTransform, const Physics& newPhysics, Graphics::MeshType meshType);

		// Destructor
		~Entity();

		// Get the transform component
		const Transform& GetTransform();

		// Set the transform component
		void SetTransform(const Transform& newTransform);

		// Get the physics component
		const Physics& GetPhysics();

		// Set the physics component
		void SetPhysics(const Physics& newPhysics);

		// Get the mesh type
		Graphics::MeshType GetMeshType() const;

		// Set the mesh type
		void SetMeshType(Graphics::MeshType newMeshType);

		// Render the entity
		void EntityRender();

		// Update the entity
		void EntityUpdate(float deltaTime);

	};

} // Entity

#endif // ENTITY_H
