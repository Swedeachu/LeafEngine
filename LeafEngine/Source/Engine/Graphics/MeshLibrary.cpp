#include "MeshLibrary.h"
#include "..\LeafEngine.h"

// get our instance of the leaf engine
#include "..\EngineInstance.h"

namespace Graphics
{

	MeshLibrary::MeshLibrary()
	{
		
	}

	// frees all the meshes
	MeshLibrary::~MeshLibrary()
	{

	}

	/*
		Creates the following Mesh's our objects will use to be rendered with from our mesh map:
		Quad Mesh 1x1, should be used for drawing single cell images and rectangles
	*/
	void MeshLibrary::InitMeshLibrary()
	{
		meshMap[QuadMesh].CreateQuad(Engine::EngineInstance.GetDirectWrapper(), DirectX::XMFLOAT2{ 1, 1 });
	}

	/*
		Gets the desired mesh type to render with of these supported default types:
		QuadMesh
	*/
	const Mesh& MeshLibrary::GetMesh(MeshType type) const
	{
		auto it = meshMap.find(type);
		if (it != meshMap.end())
		{
			return it->second;
		}
		// defaul returned mesh is quad mesh
		return meshMap.at(QuadMesh);
	}

} // Graphics
