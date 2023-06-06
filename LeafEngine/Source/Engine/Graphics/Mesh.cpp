#include "Mesh.h"

namespace Graphics
{

	Mesh::Mesh() :
		vertexBuffer(nullptr),
		vertexCount(0),
		stride(0),
		offset(0),
		inputLayout(nullptr),
		vertexShader(nullptr),
		pixelShader(nullptr),
		constantBuffer(nullptr)
	{}

	Mesh::~Mesh()
	{
		if (vertexBuffer)
		{
			vertexBuffer->Release();
		}
		if (inputLayout)
		{
			inputLayout->Release();
		}
		if (constantBuffer)
		{
			constantBuffer->Release();
		}
	}

	Mesh Mesh::CreateQuad(Graphics::DirectWrapper& directWrapper, const DirectX::XMFLOAT2& size)
	{
		// Define the vertices of the quad
		Vertex vertices[4] = {
				{ DirectX::XMFLOAT2(-size.x / 2.0f, -size.y / 2.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
				{ DirectX::XMFLOAT2(size.x / 2.0f, -size.y / 2.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
				{ DirectX::XMFLOAT2(size.x / 2.0f, size.y / 2.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
				{ DirectX::XMFLOAT2(-size.x / 2.0f, size.y / 2.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }
		};

		// Create a new mesh
		Mesh quadMesh;
		quadMesh.Initialize(directWrapper, vertices, 4);

		return quadMesh;
	}

	void Mesh::Initialize(Graphics::DirectWrapper& directWrapper, const Vertex* vertices, UINT vertexCount)
	{
		this->vertexCount = vertexCount;

		// Calculate the size of the vertex buffer
		UINT bufferSize = sizeof(Vertex) * vertexCount;

		// Create the vertex buffer
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = bufferSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vertexBufferData = {};
		vertexBufferData.pSysMem = vertices;
		directWrapper.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

		// Set the vertex buffer properties
		stride = sizeof(Vertex);
		offset = 0;

		// Create and set the input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMFLOAT2) + sizeof(DirectX::XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		directWrapper.GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), directWrapper.GetVertexShaderBlob()->GetBufferPointer(), directWrapper.GetVertexShaderBlob()->GetBufferSize(), &inputLayout);

		// Set the shaders
		vertexShader = directWrapper.GetVertexShader();
		pixelShader = directWrapper.GetPixelShader();

		// Use the constant buffer from DirectWrapper
		constantBuffer = directWrapper.GetConstantBuffer();
	}

	void Mesh::Draw(Graphics::DirectWrapper& directWrapper)
	{
		// Set the vertex buffer
		directWrapper.GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		// Set the input layout
		directWrapper.GetDeviceContext()->IASetInputLayout(inputLayout);

		// Set the primitive topology
		directWrapper.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Set the vertex shader
		directWrapper.GetDeviceContext()->VSSetShader(vertexShader, nullptr, 0);

		// Set the pixel shader
		directWrapper.GetDeviceContext()->PSSetShader(pixelShader, nullptr, 0);

		// Set the constant buffer, if applicable
		if (constantBuffer)
		{
			directWrapper.GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);
		}

		// Draw the mesh
		directWrapper.GetDeviceContext()->Draw(vertexCount, 0);
	}

} // Graphics
