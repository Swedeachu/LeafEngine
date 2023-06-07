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

			// convert it into a directX matrix
			DirectX::XMFLOAT4X4 worldMatrix = transformMatrix.ToDirectXMatrix();

			// Get the camera
			Graphics::Camera& camera = Engine::EngineInstance.GetCamera();

			// Get the view and projection matrices
			DirectX::XMFLOAT4X4 viewMatrix = camera.GetViewMatrix();
			DirectX::XMFLOAT4X4 projectionMatrix = camera.GetProjectionMatrix();

			// Create the worldViewProjection matrix
			DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&worldMatrix);
			DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&viewMatrix);
			DirectX::XMMATRIX projection = DirectX::XMLoadFloat4x4(&projectionMatrix);
			DirectX::XMMATRIX worldViewProjection = world * view * projection;

			DirectX::XMFLOAT4X4 wvp;
			DirectX::XMStoreFloat4x4(&wvp, worldViewProjection);

			// setup constant buffer
			Graphics::DirectWrapper::ConstantBufferData cbData;
			cbData.worldViewProjection = wvp;
			cbData.transform = worldMatrix;
			cbData.tintColor = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); // no tint for now
			cbData.texOffset = DirectX::XMFLOAT2(0.0f, 0.0f);
			cbData.alpha = 1.0f;

			// Get the DirectWrapper instance
			Graphics::DirectWrapper& directWrapper = Engine::EngineInstance.GetDirectWrapper();

			// Set the constant buffer data in the direct wrappers constant buffer
			directWrapper.SetConstantBufferData(cbData);

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
