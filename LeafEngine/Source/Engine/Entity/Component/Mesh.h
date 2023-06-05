#ifndef MESH_H
#define MESH_H

#include "..\..\LeafMath\Vector2D.h"
#include "..\..\Graphics\DirectWrapper.h"

namespace Component
{

	class Mesh
	{

	public:

		Mesh();
		~Mesh();

		void Initialize(Graphics::DirectWrapper& directWrapper, const DirectX::XMFLOAT2* vertices, UINT vertexCount);
		void Draw(Graphics::DirectWrapper& directWrapper);

	private:

		ID3D11Buffer* vertexBuffer;
		UINT vertexCount;
		UINT stride;
		UINT offset;
		ID3D11InputLayout* inputLayout;
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11Buffer* constantBuffer;

	};

} // Component

#endif // MESH_H
