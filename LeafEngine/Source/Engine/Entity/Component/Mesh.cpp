#include "Mesh.h"

namespace Component
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
		/*
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
		*/
	}

	void Mesh::Initialize(Graphics::DirectWrapper& directWrapper, const DirectX::XMFLOAT2* vertices, UINT vertexCount)
	{
		this->vertexCount = vertexCount;

		// Calculate the size of the vertex buffer
		UINT bufferSize = sizeof(DirectX::XMFLOAT2) * vertexCount;

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
		stride = sizeof(DirectX::XMFLOAT2);
		offset = 0;

		// Create and set the input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		directWrapper.GetDevice()->CreateInputLayout(inputElementDesc, 1, directWrapper.GetVertexShaderBlob()->GetBufferPointer(), directWrapper.GetVertexShaderBlob()->GetBufferSize(), &inputLayout);

		// Set the shaders
		vertexShader = directWrapper.GetVertexShader();
		pixelShader = directWrapper.GetPixelShader();

		// Create the constant buffer, if needed
		// Note: Adjust this part based on your requirements for constant buffers
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.ByteWidth = sizeof(Graphics::DirectWrapper::ConstantBufferData);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;
		constantBufferDesc.MiscFlags = 0;
		directWrapper.GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
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

} // Component
