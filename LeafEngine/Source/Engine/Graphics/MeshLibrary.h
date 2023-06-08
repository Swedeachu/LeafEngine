#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H

#include "Mesh.h"
#include <map>
#include <string>

namespace Graphics
{

	class MeshLibrary
	{

	public:

		MeshLibrary();
		~MeshLibrary();

		// sets all the meshes in our mesh map
		void InitMeshLibrary();

		void CreateMesh(const std::string& meshName, const std::vector<Vertex>& vertexList);

		// create a quad mesh
		void CreateQuad(const std::string& meshName, float xHalfSize, float yHalfSize, float uSize, float vSize);

		// get a mesh by name from the internal mesh map, if not found QuadMesh is returned by default
		Mesh* GetMesh(const std::string& meshName);

	private:

		// map of meshes, each one has a name
		std::map<std::string, Mesh*> meshMap;

		// adds a mesh with a name to the mesh map
		void MeshMapAdd(const std::string& meshName, Mesh* mesh);

	};

}

#endif // MESH_LIBRARY_H
