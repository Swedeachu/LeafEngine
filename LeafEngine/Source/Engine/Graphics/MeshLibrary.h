#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H

#include "Mesh.h"
#include <map>

namespace Graphics
{

	enum MeshType { QuadMesh };

	class MeshLibrary
	{
	public:

		MeshLibrary();
		~MeshLibrary();

		// sets all the meshes in our mesh map
		void InitMeshLibrary();

		const Mesh& GetMesh(MeshType type) const;

	private:

		std::map<MeshType, Mesh> meshMap;

	};

}

#endif // MESH_LIBRARY_H
