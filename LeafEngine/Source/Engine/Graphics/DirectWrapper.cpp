#include "DirectWrapper.h"
#include <fstream>

namespace Graphics
{

	ComPtr<ID3D12PipelineState> createPipeline(ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12Device> device)
	{
		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// Create pipeline state.
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = rootSignature.Get();

		auto vertexShader = DX::ReadData(L"VertexShader.cso");
		auto pixelShader = DX::ReadData(L"PixelShader.cso");

		psoDesc.VS = { vertexShader.data(), vertexShader.size() };
		psoDesc.PS = { pixelShader.data(), pixelShader.size() };

		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		psoDesc.DepthStencilState.DepthEnable = TRUE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		ComPtr<ID3D12PipelineState> pipelineState;
		if (FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState))))
		{
			// Handle error
			return nullptr;
		}

		return pipelineState;
	}

	ComPtr<IDXGISwapChain3> createSwapChain(Engine::LeafEngine& engine, ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> commandQueue)
	{
		// Construct the swap chain and its values
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = 2; // Double buffering
		swapChainDesc.Width = engine.GetWindowWidth();
		swapChainDesc.Height = engine.GetWindowHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> tempSwapChain;
		if (FAILED(factory->CreateSwapChainForHwnd(commandQueue.Get(), engine.GetEngineWindowHandle(), &swapChainDesc, nullptr, nullptr, &tempSwapChain)))
		{
			return nullptr;
		}

		ComPtr<IDXGISwapChain3> swapChain;
		if (FAILED(tempSwapChain.As(&swapChain)))
		{
			return nullptr;
		}

		// Retrieve the back buffer resources from the swap chain
		std::vector<ComPtr<ID3D12Resource>> backBuffers(swapChainDesc.BufferCount);
		for (UINT i = 0; i < swapChainDesc.BufferCount; ++i)
		{
			if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]))))
			{
				// Handle error
				return nullptr;
			}
		}

		// Set the back buffer resources in the engine
		engine.SetBackBuffers(backBuffers);

		// Create a vector to hold the command allocators and command lists
		// Modify here to create command allocators and command lists for each frame
		UINT frameCount = 2;
		std::vector<ComPtr<ID3D12CommandAllocator>> commandAllocators(frameCount);
		std::vector<ComPtr<ID3D12GraphicsCommandList>> commandLists(frameCount);

		for (UINT i = 0; i < frameCount; ++i)
		{
			ComPtr<ID3D12CommandAllocator> commandAllocator;
			if (FAILED(engine.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator))))
			{
				// Handle error
				return nullptr;
			}
			commandAllocators[i] = commandAllocator;

			ComPtr<ID3D12GraphicsCommandList> commandList;
			if (FAILED(engine.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList))))
			{
				// Handle error
				return nullptr;
			}
			commandList->Close();
			commandLists[i] = commandList;
		}

		// Set the command allocators and command lists in the engine
		engine.SetCommandAllocators(commandAllocators);
		engine.SetCommandLists(commandLists);

		return swapChain;
	}

	ComPtr<ID3D12RootSignature> createRootSignature(ComPtr<ID3D12Device> device)
	{
		D3D12_DESCRIPTOR_RANGE ranges[2] = {};

		// CBV
		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		ranges[0].NumDescriptors = 1; // We have one constant buffer
		ranges[0].BaseShaderRegister = 0; // Start from b0 register
		ranges[0].RegisterSpace = 0;
		ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// SRV
		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		ranges[1].NumDescriptors = 1; // We have one texture
		ranges[1].BaseShaderRegister = 0; // Start from t0 register
		ranges[1].RegisterSpace = 0;
		ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		D3D12_ROOT_PARAMETER rootParameters[2] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[0].DescriptorTable.pDescriptorRanges = &ranges[0];
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[1].DescriptorTable.pDescriptorRanges = &ranges[1];
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.NumParameters = _countof(rootParameters);
		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)))
		{
			// Handle error
			return nullptr;
		}

		ComPtr<ID3D12RootSignature> rootSignature;
		if (FAILED(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature))))
		{
			// Handle error
			return nullptr;
		}

		return rootSignature;
	}

	struct Vertex
	{
		DirectX::XMFLOAT3 position; // The position of the vertex
		DirectX::XMFLOAT4 color;    // The color of the vertex
	};

	void VerticeIndexBuffer(Engine::LeafEngine& engine, ComPtr<ID3D12Device> device, float aspectRatio)
	{
		// Define some vertices and indices.
		Vertex triangleVertices[] = {
				{ { 0.0f, 0.25f * aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
				{ { 0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
				{ { -0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};
		UINT16 triangleIndices[] = { 0, 1, 2 }; // important

		// Create vertex buffer.
		const UINT vertexBufferSize = sizeof(triangleVertices);
		CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		ComPtr<ID3D12Resource> vertexBuffer;
		if (FAILED(device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &vertexBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer))))
		{
			// Handle error
			return;
		}

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin = nullptr;
		CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
		if (FAILED(vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))))
		{
			// Handle error
			return;
		}
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(Vertex);
		vertexBufferView.SizeInBytes = vertexBufferSize;

		// Set the vertex buffer view in the engine
		engine.SetVertexBufferView(vertexBufferView);

		// Set the vertex buffer in the engine
		engine.SetVertexBuffer(vertexBuffer);

		// Create index buffer.
		const UINT indexBufferSize = sizeof(triangleIndices);
		CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
		ComPtr<ID3D12Resource> indexBuffer;
		if (FAILED(device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &indexBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer))))
		{
			// Handle error
			return;
		}

		// Copy the triangle data to the index buffer.
		UINT8* pIndexDataBegin = nullptr;
		if (FAILED(indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))))
		{
			// Handle error
			return;
		}
		memcpy(pIndexDataBegin, triangleIndices, sizeof(triangleIndices));
		indexBuffer->Unmap(0, nullptr);

		// Initialize the index buffer view.
		D3D12_INDEX_BUFFER_VIEW indexBufferView = {};
		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		indexBufferView.SizeInBytes = indexBufferSize;

		// Set the index buffer view in the engine
		engine.SetIndexBufferView(indexBufferView);

		// Set the index buffer in the engine
		engine.SetIndexBuffer(indexBuffer);
	}

	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}

		*ppAdapter = adapter.Detach();
	}

	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Type = type;

		// Only make CBV/SRV/UAV heaps shader visible
		if (type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		{
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		}
		else
		{
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		}

		ComPtr<ID3D12DescriptorHeap> descriptorHeap;
		HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf()));

		if (FAILED(hr))
		{
			// Handle error
			std::cout << "Failed to create descriptor heap. HRESULT = " << hr << std::endl;
			return nullptr;
		}

		return descriptorHeap;
	}

	void InitializeRTVDescriptorHeap(Engine::LeafEngine& engine)
	{
		ComPtr<ID3D12Device> device = engine.GetDevice();
		// Get the descriptor handle increment size for RTV.
		UINT rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		// Create a RTV descriptor heap.
		ComPtr<ID3D12DescriptorHeap> rtvHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2);
		// Save the RTV heap in the engine instance.
		engine.SetRtvHeap(rtvHeap);
		// Create a CPU descriptor handle for the RTV heap start.
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// Set the RTV handles in the engine for each back buffer.
		std::vector<ComPtr<ID3D12Resource>>& backBuffers = engine.GetBackBuffers();
		for (UINT i = 0; i < backBuffers.size(); ++i)
		{
			// Create a RTV for the back buffer resource.
			device->CreateRenderTargetView(backBuffers[i].Get(), nullptr, rtvHandle);
			// Increment the RTV handle.
			rtvHandle.ptr += rtvDescriptorSize;
		}
	}

	struct ConstantBufferData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	// Assume this is called in InitializeCBVDescriptorHeap
	void InitializeConstantBuffer(Engine::LeafEngine& engine, ConstantBufferData& cbData)
	{
		ComPtr<ID3D12Device> device = engine.GetDevice();

		// Create the constant buffer.
		UINT64 bufferSize = (sizeof(ConstantBufferData) + 255) & ~255;  // Align the buffer size to multiples of 256.

		ComPtr<ID3D12Resource> constantBuffer;
		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = bufferSize;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		HRESULT hr = device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constantBuffer)
		);
		if (FAILED(hr))
		{
			// Handle error
			return;
		}

		// Copy the data to the constant buffer.
		ConstantBufferData* pData;
		hr = constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));
		if (FAILED(hr))
		{
			// Handle error
			return;
		}
		memcpy(pData, &cbData, sizeof(cbData));
		constantBuffer->Unmap(0, nullptr);

		// Save the constant buffer in the engine instance
		engine.SetConstantBuffer(constantBuffer);
	}

	void InitializeCBVDescriptorHeap(Engine::LeafEngine& engine, ConstantBufferData& cbData)
	{
		ComPtr<ID3D12Device> device = engine.GetDevice();
		// Assume you have 1 CBV.
		ComPtr<ID3D12DescriptorHeap> cbvHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
		// Save the CBV heap in the engine instance
		engine.SetCbvHeap(cbvHeap);

		// Initialize the constant buffer and save it in the engine instance
		InitializeConstantBuffer(engine, cbData);
		ComPtr<ID3D12Resource> constantBuffer = engine.GetConstantBuffer();

		// Create a CBV.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (sizeof(ConstantBufferData) + 255) & ~255;  // Must be a multiple of 256
		device->CreateConstantBufferView(&cbvDesc, cbvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void InitializeDSVDescriptorHeap(Engine::LeafEngine& engine)
	{
		ComPtr<ID3D12Device> device = engine.GetDevice();
		// Create a DSV descriptor heap.
		ComPtr<ID3D12DescriptorHeap> dsvHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
		// Save the DSV heap in the engine instance
		engine.SetDsvHeap(dsvHeap);
	}

	void CreateDepthStencilView(Engine::LeafEngine& engine)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
		depthOptimizedClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
		depthOptimizedClearValue.DepthStencil.Stencil = 0;

		ComPtr<ID3D12Resource> depthStencil;

		CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			engine.GetWindowWidth(),
			engine.GetWindowHeight(),
			1, // this is the mipLevels parameter. For a Depth/Stencil buffer, this should be 1.
			1, // this is the arraySize parameter. For a Depth/Stencil buffer, this should be 1.
			1, // this is the sampleCount parameter. For a Depth/Stencil buffer, this should be 1.
			0, // this is the sampleQuality parameter. For a Depth/Stencil buffer, this should be 0.
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL // this sets the resource to be used as a depth stencil target.
		);


		HRESULT hr = engine.GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthOptimizedClearValue,
			IID_PPV_ARGS(&depthStencil)
		);

		if (FAILED(hr))
		{
			// Handle error
			std::cout << "Failed to create depth stencil resource. HRESULT = " << hr << std::endl;
			return;
		}

		// Save the depthStencil in the engine instance
		engine.SetDepthStencil(depthStencil);
		engine.GetDevice()->CreateDepthStencilView(engine.GetDepthStencil().Get(), &dsvDesc, engine.GetDsvHeap()->GetCPUDescriptorHandleForHeapStart());
	}

	// Create the render target views for the swap chain back buffers.
	void CreateRenderTargetViews(Engine::LeafEngine& engine)
	{
		// Retrieve the descriptor size for RTV.
		UINT rtvDescriptorSize = engine.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		// Get the handle to the first descriptor in the RTV heap.
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = engine.GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();

		// Clear the existing render targets
		engine.GetRenderTargets().clear();

		for (UINT i = 0; i < 2; i++)
		{
			ComPtr<ID3D12Resource> backBuffer;
			HRESULT hr = engine.GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&backBuffer));
			if (FAILED(hr))
			{
				// Handle error
				return;
			}

			// Create a RTV for the swap chain back buffer.
			engine.GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
			// Increment the RTV handle.
			rtvHandle.ptr += rtvDescriptorSize;

			// Add the back buffer to the render targets vector
			engine.GetRenderTargets().emplace_back(backBuffer);
		}
	}

	void InitializeFence(Engine::LeafEngine& engine, ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> commandQueue)
	{
		const int frameCount = 2;

		std::vector<ComPtr<ID3D12Fence>> fences(frameCount);
		std::vector<UINT64> fenceValues(frameCount, 0); // Initialize to 0

		for (int i = 0; i < frameCount; ++i)
		{
			if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fences[i]))))
			{
				// Handle error
				throw std::runtime_error("Failed to create fence for GPU synchronization.");
			}

			// Do not signal the fence immediately after creation
		}

		engine.SetFences(fences);
		engine.SetFenceValues(fenceValues);

		// Create the fence event
		HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (fenceEvent == nullptr)
		{
			// Handle error
			throw std::runtime_error("Failed to create fence event.");
		}

		engine.SetFenceEvent(fenceEvent);
	}

	void InitializeDirectX12(Engine::LeafEngine& engine)
	{
		UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
		{
			// Enable debug layer
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
#endif

		// Create DXGI factory
		ComPtr<IDXGIFactory4> factory;
		if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory))))
		{
			// Handle error
			return;
		}

		// Get hardware adapter
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(factory.Get(), &hardwareAdapter);

		// Create D3D12 device
		ComPtr<ID3D12Device> device;
		if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
		{
			// Handle error
			return;
		}

		engine.SetDevice(device);

		// Create command queue
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		ComPtr<ID3D12CommandQueue> commandQueue;
		if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue))))
		{
			// Handle error
			return;
		}
		engine.SetCommandQueue(commandQueue);

		// Initialize fence
		InitializeFence(engine, device, commandQueue);

		// Create swap chain
		ComPtr<IDXGISwapChain3> swapChain = createSwapChain(engine, factory, commandQueue);
		engine.SetSwapChain(swapChain);

		// Initialize descriptor heaps
		ConstantBufferData cbData = {};
		InitializeRTVDescriptorHeap(engine);
		InitializeCBVDescriptorHeap(engine, cbData);
		InitializeDSVDescriptorHeap(engine);

		// Create root signature and pipeline state
		ComPtr<ID3D12RootSignature> rootSignature = createRootSignature(device);
		engine.SetRootSignature(rootSignature);
		ComPtr<ID3D12PipelineState> pipeline = createPipeline(rootSignature, device);
		engine.SetPipelineState(pipeline);

		// Create vertex and index buffers
		VerticeIndexBuffer(engine, device, (float)(engine.GetWindowWidth() / engine.GetWindowHeight()));

		// Create render target views
		CreateRenderTargetViews(engine);

		// Create depth/stencil view
		CreateDepthStencilView(engine);
	}

} // Graphics
