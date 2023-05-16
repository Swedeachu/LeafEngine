#include "stdafx.h"
#include "LeafEngine.h"
#include "GameScene/GameSceneSystem.h"
#include "System/GameSystem.h"
#include "Graphics/DirectWrapper.h"

namespace Engine
{

	// construct an instance of the leaf engine
	LeafEngine::LeafEngine(int fps, int width, int height, const std::wstring& title) : frameRate(fps), windowWidth(width), windowHeight(height), windowTitle(title)
	{
		windowClassName = L"LeafEngineDirectX12WindowClass";
	}

	LeafEngine::~LeafEngine()
	{
		if (fenceEvent != nullptr)
		{
			// CloseHandle(fenceEvent);
		}
		// Release other resources...
	}

	// stupid hack around to get the windows callback function we want for setting the windows class
	LRESULT CALLBACK LeafEngine::StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Retrieve the pointer to the LeafEngine instance
		LeafEngine* enginePtr = reinterpret_cast<LeafEngine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		// Call the non-static member function for window procedure
		return enginePtr->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	// basic message reciever
	LRESULT CALLBACK LeafEngine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	// runs the leaf engine at the set frame rate for the created window the engine was assigned to
	int LeafEngine::Run()
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		// Register window class
		WNDCLASS wc = {};
		wc.lpfnWndProc = StaticWindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = windowClassName.c_str();

		RegisterClass(&wc);

		// Create window
		engineWindowHandle = CreateWindowEx(
			0,
			windowClassName.c_str(),
			windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		if (engineWindowHandle == nullptr)
		{
			return 1; // error if the handle isn't made correctly
		}

		// Set up the DirectX device and other needed things on the LeafEngine instance
		Graphics::InitializeDirectX12(*this);

		// Show the window
		ShowWindow(engineWindowHandle, SW_SHOW);

		// Calculate the desired frame time
		const double frameTime = 1.0 / frameRate;

		// Enter the message loop
		MSG msg = {};
		double deltaTime = 0.0;
		double totalTime = 0.0;
		LARGE_INTEGER frequency, prevTime, currTime;

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&prevTime);

		// init the systems
		//internalGameSystem.InitSystems();

		// GameSceneSystem sceneSystem = GameSceneSystem();
		while (msg.message != WM_QUIT)
		{
			// Handle window messages
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// Calculate delta time and total time
				QueryPerformanceCounter(&currTime);
				deltaTime = static_cast<double>(currTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
				prevTime = currTime;
				totalTime += deltaTime;
				// Check if it's time to update the screen
				if (totalTime >= frameTime)
				{
					// first update all internal systems
					//internalGameSystem.UpdateSystems();

					RenderFrame();

					// Subtract frame time from total time
					totalTime -= frameTime;
					// Update total amount of frames
					totalFrames++;
				}
			}
		}

		// exit all systems once out of main loop
		//internalGameSystem.ExitSystems();

		return static_cast<int>(msg.wParam);
	}

	void LeafEngine::RenderFrame()
	{
#if defined(_DEBUG)
		std::cout << "Rendering Frame " << GetTotalFrames() << " at frame index " << frameIndex << std::endl;
#endif

		// Wait for the GPU to finish presenting the previous frame that used the same back buffer
		auto fence = fences[frameIndex].Get();
		const UINT64 fenceValue = fenceValues[frameIndex];
		if (fence->GetCompletedValue() < fenceValue)
		{
			HANDLE event = GetFenceEvent();
			fence->SetEventOnCompletion(fenceValue, event);
			WaitForSingleObject(event, INFINITE);
		}

		// Command list allocation
		auto allocator = commandAllocators[frameIndex].Get();
		HRESULT hr = allocator->Reset();
		if (FAILED(hr))
		{
			std::wcout << "Failed to reset command allocator: " << hr << std::endl;
			return;
		}

		// Get the index of the current back buffer
		UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();

		// Command list recording
		auto commandList = commandLists[frameIndex].Get();
		hr = commandList->Reset(allocator, pipelineState.Get());
		if (FAILED(hr))
		{
			std::wcout << "Failed to reset command list: " << hr << std::endl;
			return;
		}

		// Set the necessary state
		commandList->SetGraphicsRootSignature(rootSignature.Get());

		// Set up the viewport and scissor rect
		commandList->RSSetViewports(1, &viewport);
		commandList->RSSetScissorRects(1, &scissorRect);

		// Indicate that the back buffer will be used as the render target
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = backBuffers[backBufferIndex].Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrier);

		// Clear the render target with the random color
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), backBufferIndex, rtvDescriptorSize);
		float color[4] = { 1.0, 1.0, 1.0, 1.0 };
		commandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);

		// Record commands
		// Here you would normally record your actual rendering commands
		// For example, draw geometry, etc.

		// Indicate that the back buffer will now be used to present
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		commandList->ResourceBarrier(1, &barrier);

		// Close the command list
		hr = commandList->Close();
		if (FAILED(hr))
		{
			std::wcout << "Failed to close command list: " << hr << std::endl;
			return;
		}

		// Execute the command list
		ID3D12CommandList* ppCommandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame
		hr = swapChain->Present(0, 0);
		if (FAILED(hr))
		{
			std::wcout << "Failed to present swap chain: " << hr << std::endl;
			return;
		}

		// Signal the fence for the current frame
		hr = commandQueue->Signal(fence, fenceValue + 1);
		if (FAILED(hr))
		{
			std::wcout << "Failed to signal command queue: " << hr << std::endl;
			return;
		}

		// Update the fence value for the current frame
		fenceValues[frameIndex] = fenceValue + 1;

		// Alternate the frame index for double buffering
		frameIndex = (frameIndex + 1) % 2;
	}

	void LeafEngine::OnResize(UINT width, UINT height)
	{
		if (device == nullptr)
		{
			return;
		}

		// Update the aspect ratio
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		// Resize the swap chain
		HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr))
		{
			// Handle error
			return;
		}

		// Update the render target views and depth stencil view
		Graphics::CreateRenderTargetViews(*this);
		Graphics::CreateDepthStencilView(*this);

		// Set the viewport and scissor rect
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		scissorRect.right = static_cast<LONG>(width);
		scissorRect.bottom = static_cast<LONG>(height);
	}

#pragma region Window properties

	int LeafEngine::GetFrameRate() const
	{
		return frameRate;
	}

	void LeafEngine::SetFrameRate(const int fps)
	{
		frameRate = fps;
	}

	int LeafEngine::GetTotalFrames() const
	{
		return totalFrames;
	}

	int LeafEngine::GetWindowWidth() const
	{
		return windowWidth;
	}

	int LeafEngine::GetWindowHeight() const
	{
		return windowHeight;
	}

	// Changes the window's size
	void LeafEngine::SetWindowSize(const int width, const int height)
	{
		windowWidth = width;
		windowHeight = height;

		if (engineWindowHandle != nullptr)
		{
			// Adjust the window size
			RECT rect = { 0, 0, windowWidth, windowHeight };
			AdjustWindowRect(&rect, GetWindowLong(engineWindowHandle, GWL_STYLE), FALSE);
			SetWindowPos(engineWindowHandle, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}

	// Highly reccomended to check Leaf Engine documentation before trying to mess around with the window handle.
	// We probably already have a function for what you are trying to do with the engine's window.
	HWND LeafEngine::GetEngineWindowHandle() const
	{
		return engineWindowHandle;
	}

#pragma endregion end of window properties

#pragma region Graphical getters and setters

	ComPtr<ID3D12Device> LeafEngine::GetDevice() const
	{
		return device;
	}

	void LeafEngine::SetDevice(ComPtr<ID3D12Device> device)
	{
		this->device = device;
	}

	ComPtr<ID3D12CommandQueue> LeafEngine::GetCommandQueue() const
	{
		return commandQueue;
	}

	void LeafEngine::SetCommandQueue(const ComPtr<ID3D12CommandQueue>& queue)
	{
		commandQueue = queue;
	}

	std::vector<ComPtr<ID3D12GraphicsCommandList>> LeafEngine::GetCommandLists() const
	{
		return commandLists;
	}

	void LeafEngine::SetCommandLists(const std::vector<ComPtr<ID3D12GraphicsCommandList>>& newCommandLists)
	{
		commandLists = newCommandLists;
	}

	std::vector<ComPtr<ID3D12CommandAllocator>> LeafEngine::GetCommandAllocators() const
	{
		return commandAllocators;
	}

	void LeafEngine::SetCommandAllocators(const std::vector<ComPtr<ID3D12CommandAllocator>>& commandAllocators)
	{
		this->commandAllocators = commandAllocators;
	}

	const std::vector<ComPtr<ID3D12Resource>>& LeafEngine::GetRenderTargets() const
	{
		return renderTargets;
	}

	void LeafEngine::SetRenderTargets(const std::vector<ComPtr<ID3D12Resource>>& renderTargets)
	{
		this->renderTargets = renderTargets;
	}

	ComPtr<ID3D12RootSignature> LeafEngine::GetRootSignature() const
	{
		return rootSignature;
	}

	void LeafEngine::SetRootSignature(const ComPtr<ID3D12RootSignature>& newRootSignature)
	{
		rootSignature = newRootSignature;
	}

	ComPtr<ID3D12PipelineState> LeafEngine::GetPipelineState() const
	{
		return pipelineState;
	}

	void LeafEngine::SetPipelineState(const ComPtr<ID3D12PipelineState>& newPipelineState)
	{
		pipelineState = newPipelineState;
	}

	void LeafEngine::SetFences(std::vector<ComPtr<ID3D12Fence>> fences)
	{
		this->fences = fences;
	}

	std::vector<ComPtr<ID3D12Fence>>& LeafEngine::GetFences()
	{
		return fences;
	}

	void  LeafEngine::SetFenceValues(std::vector<UINT64> fenceValues)
	{
		this->fenceValues = fenceValues;
	}
	std::vector<UINT64>& LeafEngine::GetFenceValues()
	{
		return fenceValues;
	}

	HANDLE LeafEngine::GetFenceEvent() const
	{
		return fenceEvent;
	}

	void LeafEngine::SetFenceEvent(HANDLE event)
	{
		fenceEvent = event;
	}

	UINT LeafEngine::GetFrameIndex() const
	{
		return frameIndex;
	}

	void LeafEngine::SetFrameIndex(UINT index)
	{
		frameIndex = index;
	}

	ComPtr<IDXGISwapChain3> LeafEngine::GetSwapChain() const
	{
		return swapChain;
	}

	void LeafEngine::SetSwapChain(const ComPtr<IDXGISwapChain3>& chain)
	{
		swapChain = chain;
	}

	void LeafEngine::SetBackBuffers(std::vector<ComPtr<ID3D12Resource>> backBuffers)
	{
		this->backBuffers = backBuffers;
	}

	std::vector<ComPtr<ID3D12Resource>>& LeafEngine::GetBackBuffers()
	{
		return backBuffers;
	}

	void LeafEngine::SetConstantBuffer(const ComPtr<ID3D12Resource>& constantBuffer)
	{
		m_constantBuffer = constantBuffer;
	}

	ComPtr<ID3D12Resource> LeafEngine::GetConstantBuffer() const
	{
		return m_constantBuffer;
	}

	ComPtr<ID3D12DescriptorHeap> LeafEngine::GetRtvHeap() const
	{
		return rtvHeap;
	}

	void LeafEngine::SetRtvHeap(const ComPtr<ID3D12DescriptorHeap>& rtvHeap)
	{
		this->rtvHeap = rtvHeap;
	}

	ComPtr<ID3D12DescriptorHeap> LeafEngine::GetDsvHeap() const
	{
		return dsvHeap;
	}

	void LeafEngine::SetDsvHeap(const ComPtr<ID3D12DescriptorHeap>& newDsvHeap)
	{
		dsvHeap = newDsvHeap;
	}

	ComPtr<ID3D12DescriptorHeap> LeafEngine::GetCbvHeap() const
	{
		return this->cbvHeap;
	}

	void LeafEngine::SetCbvHeap(const ComPtr<ID3D12DescriptorHeap>& cbvHeap)
	{
		this->cbvHeap = cbvHeap;
	}

	ComPtr<ID3D12Resource> LeafEngine::GetDepthStencil() const
	{
		return depthStencil;
	}

	void LeafEngine::SetDepthStencil(const ComPtr<ID3D12Resource>& newDepthStencil)
	{
		depthStencil = newDepthStencil;
	}

	void LeafEngine::SetVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW& view)
	{
		this->vertexBufferView = view;
	}

	D3D12_VERTEX_BUFFER_VIEW LeafEngine::GetVertexBufferView() const
	{
		return this->vertexBufferView;
	}

	void LeafEngine::SetIndexBufferView(const D3D12_INDEX_BUFFER_VIEW& view)
	{
		this->indexBufferView = view;
	}

	D3D12_INDEX_BUFFER_VIEW LeafEngine::GetIndexBufferView() const
	{
		return this->indexBufferView;
	}

	void LeafEngine::SetVertexBuffer(ComPtr<ID3D12Resource> vb)
	{
		vertexBuffer = vb;
	}

	ComPtr<ID3D12Resource> LeafEngine::GetVertexBuffer() const
	{
		return vertexBuffer;
	}

	void LeafEngine::SetIndexBuffer(ComPtr<ID3D12Resource> ib)
	{
		indexBuffer = ib;
	}

	ComPtr<ID3D12Resource> LeafEngine::GetIndexBuffer() const
	{
		return indexBuffer;
	}

	D3D12_VIEWPORT LeafEngine::GetViewport() const
	{
		return viewport;
	}

	void LeafEngine::SetViewport(const D3D12_VIEWPORT& vp)
	{
		viewport = vp;
	}

	D3D12_RECT LeafEngine::GetScissorRect() const
	{
		return scissorRect;
	}

	void LeafEngine::SetScissorRect(const D3D12_RECT& rect)
	{
		scissorRect = rect;
	}

	float LeafEngine::GetAspectRatio() const
	{
		return aspectRatio;
	}

	void LeafEngine::SetAspectRatio(float ratio)
	{
		aspectRatio = ratio;
	}

#pragma endregion End of Graphical getters and setters

} // Engine
