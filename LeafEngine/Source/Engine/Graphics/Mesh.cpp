#include "Mesh.h"

namespace Graphics
{

	Mesh::Mesh() : vertexBuffer(nullptr), vertexCount(0), stride(sizeof(Vertex)), offset(0)
	{}

	Mesh::~Mesh()
	{
		if (vertexBuffer)
		{
			vertexBuffer->Release();
			vertexBuffer = nullptr;
		}
	}

	void Mesh::Initialize(DirectWrapper& directWrapper, const Vertex* vertices, UINT vertexCount)
	{
		this->vertexCount = vertexCount;

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices;

		HRESULT hr = directWrapper.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create vertex buffer. HRESULT: " + std::to_string(hr));
		}
	}

	void Mesh::Draw(DirectWrapper& directWrapper)
	{
		directWrapper.GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		directWrapper.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		directWrapper.GetDeviceContext()->Draw(vertexCount, 0);
	}

	Mesh Mesh::CreateQuad(DirectWrapper& directWrapper, const DirectX::XMFLOAT2& size)
	{
		Vertex vertices[] =
		{
			// Positions     // Colors                   // Texture coordinates
			{ { 0.0f, size.y }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // Top-left
			{ { size.x, size.y }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // Top-right
			{ { size.x, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Bottom-right
			{ { 0.0f, size.y }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // Top-left
			{ { size.x, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // Bottom-right
			{ { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }  // Bottom-left
		};

		Mesh quad;
		quad.Initialize(directWrapper, vertices, sizeof(vertices) / sizeof(Vertex));
		return quad;
	}


} // Graphics
