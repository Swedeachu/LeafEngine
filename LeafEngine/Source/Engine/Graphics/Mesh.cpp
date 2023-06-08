#include "Mesh.h"

#include "..\EngineInstance.h"

namespace Graphics
{

	Mesh::Mesh(const std::vector<Vertex>& _vertexList)
	{
		// copy in the vertex list into this current mesh
		vertexCount = (unsigned)_vertexList.size();
		vertexList = new Vertex[vertexCount];
		memcpy(vertexList, _vertexList.data(), sizeof(Vertex) * vertexCount);
		// Set up the vertex buffer description struct
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// Set up the subresource data struct
		D3D11_SUBRESOURCE_DATA subResourceData = { 0 };
		subResourceData.pSysMem = vertexList;
		// Create the vertex buffer
		Engine::EngineInstance.GetDirectWrapper().GetDevice()->CreateBuffer(&vertexBufferDesc, &subResourceData, &vertexBuffer);
	}

	Mesh::~Mesh()
	{
		if (vertexBuffer)
		{
			vertexBuffer->Release();
		}
		if (indexBuffer)
		{
			indexBuffer->Release();
		}
		if (vertexList)
		{
			delete vertexList;
		}
		if (indices)
		{
			delete indices;
		}
	}

	void Mesh::Draw()
	{
		// get the needed objects
		DirectWrapper& directWrapper = Engine::EngineInstance.GetDirectWrapper();
		ID3D11DeviceContext* deviceContext = directWrapper.GetDeviceContext();
		// set topology
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// TO DO : set pixel shader based on current shader in directWrapper (color or tex pixel shader)
		// Set the shader
		deviceContext->PSSetShader(directWrapper.GetPixelShader(), NULL, 0);

		// If there is a texture, set the shader resource (texture object not implemented yet)
		//if (directWrapper.GetShaderMode() == SM_TEXTURE && texture)
		//		deviceContext->PSSetShaderResources(0, 1, &(texture->texResourceView));
		//else
		//{
		ID3D11ShaderResourceView* nullSRV = { nullptr };
		deviceContext->PSSetShaderResources(0, 1, &nullSRV);
		//}

		// set vertex buffers
		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		// update constant buffer data
		directWrapper.UpdateConstantBuffer();
		// draw the mesh
		deviceContext->Draw(vertexCount, 0);
	}

} // Graphics
