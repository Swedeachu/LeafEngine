#include "Entity.h"
#include "../Graphics/Mesh.h"
#include "../EngineInstance.h"

namespace Entity
{

	// Constructor
	Entity::Entity()
	{
		hasTransform = false;
		hasPhysics = false;
		meshTypeName = "QuadMesh";
	}

	Entity::Entity(const Transform& newTransform, const Physics& newPhysics, std::string meshTypeName) : transform(newTransform), physics(newPhysics), meshTypeName(meshTypeName)
	{
		hasTransform = true;
		hasPhysics = true;
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

	// Get the type of mesh the Entity will be rendered with
	std::string Entity::GetMeshType() const
	{
		return meshTypeName;
	}

	// Set the type of mesh the Entity will be rendered with
	void Entity::SetMeshType(std::string newMeshType)
	{
		meshTypeName = newMeshType;
	}

	// Render the entity
	void Entity::EntityRender()
	{
		if (hasTransform)
		{
			// Get our graphics wrapper
			Graphics::DirectWrapper& directWrapper = Engine::EngineInstance.GetDirectWrapper();

			// Get the transform component
			const Transform& entityTransform = GetTransform();
			// Get the transformation matrix from the transform component
			const LeafMath::Matrix2D& transformMatrix = entityTransform.GetMatrix();
			// Convert it into a directX matrix
			DirectX::XMFLOAT4X4 worldMatrix = transformMatrix.ToDirectXMatrix();
			// set the transform data for rendering the mesh
			directWrapper.SetConstantBufferTransform(&worldMatrix);

			// this does the exact same thing in less lines of code despite being computationally slower by a land slide since our Transform component class uses lazy rect
			// directWrapper.SetConstantBufferTransformData(entityTransform.GetPosition(), entityTransform.GetScale(), entityTransform.GetRotation());

			// Set constant buffer alpha and tint color
			directWrapper.SetConstantBufferAlpha(1.0f); // default 1.0 for now
			DirectX::XMFLOAT4 tintColor = { 0.0f, 0.0f, 0.0f, 0.0f }; // blank for now
			directWrapper.SetConstantBufferTintColor(&tintColor);

			// invoke the mesh library to render the selected mesh of this entity
			Engine::EngineInstance.GetMeshLibrary().GetMesh(meshTypeName)->Draw();
		}
	}

	// Update the entity
	void Entity::EntityUpdate(float deltaTime)
	{
		if (hasPhysics && hasTransform)
		{
			// physics.PhysicsUpdate(transform, deltaTime);
		}
	}

} // Entity
