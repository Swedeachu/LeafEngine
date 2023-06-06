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
		meshType = Graphics::QuadMesh;
	}

	Entity::Entity(const Transform& newTransform, const Physics& newPhysics, Graphics::MeshType meshType) : transform(newTransform), physics(newPhysics), meshType(meshType)
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
	Graphics::MeshType Entity::GetMeshType() const
	{
		return meshType;
	}

	// Set the type of mesh the Entity will be rendered with
	void Entity::SetMeshType(Graphics::MeshType newMeshType)
	{
		meshType = newMeshType;
	}

	// Render the entity
	void Entity::EntityRender()
	{
		if (hasTransform)
		{
			// Get the transform component
			const Transform& entityTransform = GetTransform();

			// Get the transformation matrix from the transform component
			const LeafMath::Matrix2D& transformMatrix = entityTransform.GetMatrix();

			// Get the DirectWrapper instance
			Graphics::DirectWrapper& directWrapper = Engine::EngineInstance.GetDirectWrapper();

			// Set the transformation matrix in the constant buffer
			Graphics::DirectWrapper::ConstantBufferData constantBufferData;

			// Convert the LeafMath::Matrix2D to DirectX::XMFLOAT4X4
			DirectX::XMFLOAT4X4 worldMatrix = transformMatrix.ToDirectXMatrix();
			DirectX::XMStoreFloat4x4(&constantBufferData.worldMatrix, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&worldMatrix)));

			// Set the color
			constantBufferData.color = { 1.0f, 0.0f, 0.0f, 1.0f }; // Set to red color

			// set the data to the constant buffer
			directWrapper.SetConstantBufferData(constantBufferData);

			// Get the mesh object
			Graphics::Mesh meshObj = Engine::EngineInstance.GetMeshLibrary().GetMesh(meshType);

			// Draw the mesh
			meshObj.Draw(directWrapper);
		}
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
