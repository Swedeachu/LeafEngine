#include "MeshLibrary.h"

#include "..\EngineInstance.h"

namespace Graphics
{

	MeshLibrary::MeshLibrary()
	{}

	MeshLibrary::~MeshLibrary()
	{
		// Clean up the mesh map
		for (auto& pair : meshMap)
		{
			delete pair.second;
		}
	}

	/*
		Creates the following Mesh's our objects will use to be rendered with from our mesh map:
		Quad Mesh 1x1, should be used for drawing single cell images and rectangles
	*/
	void MeshLibrary::InitMeshLibrary()
	{
		CreateQuad("QuadMesh", 0.5f, 0.5f, 1.0f, 1.0f);
	}

	// A quad mesh is two triangles placed perfectly together to make a square
	// This is the most versatile mesh, and will be used for sprite sheets as well
	void MeshLibrary::CreateQuad(const std::string& meshName, float xHalfSize, float yHalfSize, float uSize, float vSize)
	{
		std::vector<Vertex> vertexList;

		// First Triangle
		vertexList.push_back({ { -xHalfSize, yHalfSize }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } });
		vertexList.push_back({ { xHalfSize, -yHalfSize }, { 0.0f, 1.0f, 0.0f, 1.0f }, { uSize, vSize } });
		vertexList.push_back({ { xHalfSize, yHalfSize }, { 0.0f, 0.0f, 1.0f, 1.0f }, { uSize, 0.0f } });

		// Second Triangle
		vertexList.push_back({ { -xHalfSize, yHalfSize }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } });
		vertexList.push_back({ { -xHalfSize, -yHalfSize }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, vSize } });
		vertexList.push_back({ { xHalfSize, -yHalfSize }, { 1.0f, 0.0f, 0.0f, 1.0f }, { uSize, vSize } });

		CreateMesh(meshName, vertexList);
	}

	/*
		Creates a named mesh made up of a vertex list, which is then added to the Game Engine's Mesh memory.
		Predefined meshes you don't have to worry about implementing yourself:
		QuadMesh
	*/
	void MeshLibrary::CreateMesh(const std::string& meshName, const std::vector<Vertex>& vertexList)
	{
		// Construct a new mesh from the vertex list
		Mesh* mesh = new Mesh(vertexList);
		// Add to the mesh map in memory
		MeshMapAdd(meshName, mesh);
	}

	// add to internal structure
	void MeshLibrary::MeshMapAdd(const std::string& meshName, Mesh* mesh)
	{
		meshMap.insert(std::make_pair(meshName, mesh));
	}

	/*
		Gets the desired mesh type to render with of these supported default types:
		QuadMesh
	*/
	Mesh* MeshLibrary::GetMesh(const std::string& meshName)
	{
		auto it = meshMap.find(meshName);
		if (it != meshMap.end())
		{
			return it->second;
		}
		// Default returned mesh is QuadMesh
		return meshMap.at("QuadMesh");
	}

} // Graphics
