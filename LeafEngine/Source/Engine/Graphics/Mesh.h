#ifndef MESH_H
#define MESH_H

#include "DirectWrapper.h"

namespace Graphics
{

	struct Vertex
	{
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texCoord;
	};

	class Mesh
	{

	public:

		// this creates a new mesh based on current vertices in the list
		Mesh(const std::vector<Vertex>& _vertexList);

		~Mesh();

		void Draw();

	private:

		// the list of vertexes that make up the mesh
		Vertex* vertexList{ nullptr };
		// amount of vertices in the mesh
		unsigned vertexCount{ 0 };
		// vertex buffer
		ID3D11Buffer* vertexBuffer{ nullptr };
		// array of indices for an indexed mesh
		unsigned* indices{ nullptr };
		// the number indices in the index array
		unsigned indexCount{ 0 };
		// buffers
		ID3D11Buffer* indexBuffer{ nullptr };

		static constexpr UINT stride{ sizeof(Vertex) };
		static constexpr UINT offset{ 0 };

	};

} // Graphics

#endif // MESH_H
