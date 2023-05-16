#include "Mesh.h"

namespace Graphics
{

	/*

	Mesh::Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12PipelineState* pipelineState)
		: m_VertexBuffer(nullptr), m_VertexBufferView()
	{
		// Create vertex buffer for the square mesh
		struct Vertex
		{
			float position[3];
			float color[3];
		};

		Vertex vertices[] =
		{
			{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } }, // Vertex 0: position (-0.5, -0.5, 0.0), color (red)
			{ { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },  // Vertex 1: position (0.5, -0.5, 0.0), color (red)
			{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },  // Vertex 2: position (-0.5, 0.5, 0.0), color (red)
			{ { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } }    // Vertex 3: position (0.5, 0.5, 0.0), color (red)
		};

		const UINT vertexBufferSize = sizeof(vertices);

		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC vertexBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC uploadBufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		// Create default heap resource for vertex buffer
		device->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&vertexBufferResourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_VertexBuffer));

		// Create upload heap resource for vertex buffer
		ID3D12Resource* vertexBufferUploadHeap;
		device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&uploadBufferResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexBufferUploadHeap));

		// Copy the vertex data to the upload heap
		D3D12_SUBRESOURCE_DATA vertexData{};
		vertexData.pData = reinterpret_cast<BYTE*>(vertices);
		vertexData.RowPitch = vertexBufferSize;
		vertexData.SlicePitch = vertexBufferSize;

		UpdateSubresources(commandList, m_VertexBuffer, vertexBufferUploadHeap, 0, 0, 1, &vertexData);

		// Transition the vertex buffer to the vertex buffer state
		commandList->ResourceBarrier(1, &transitionBarrier);

		// Set up the vertex buffer view
		m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexBufferView.StrideInBytes = sizeof(Vertex);
		m_VertexBufferView.SizeInBytes = vertexBufferSize;
	}

	void Mesh::DrawMesh(ID3D12GraphicsCommandList* commandList)
	{
		// Set vertex buffer
		commandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
		// Set primitive topology
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		// Draw vertices
		commandList->DrawInstanced(4, 1, 0, 0);
	}

	*/

} // Engine