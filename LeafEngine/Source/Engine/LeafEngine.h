#ifndef LEAF_ENGINE_H
#define LEAF_ENGINE_H

#include "stdafx.h"
#include "GameScene/GameSceneSystem.h"
#include "System/GameSystem.h"

namespace Engine
{

	class LeafEngine
	{

	public:

		// create the leaf engine and its window
		LeafEngine(int fps, int width, int height, const std::wstring& title);

		// deconstructor should free and release things
		~LeafEngine();

		// run the engine
		int Run();

		// Getter and Setter for frame rate

		int GetFrameRate() const;
		void SetFrameRate(const int fps);
		int GetTotalFrames() const;

		// Getter and Setter for window size

		int GetWindowWidth() const;
		int GetWindowHeight() const;
		void SetWindowSize(const int width, const int height);

		// Get the window handle for the engine's instance
		HWND GetEngineWindowHandle() const;

		// Graphical component getters and setters

		ComPtr<ID3D12Device> GetDevice() const;
		void SetDevice(ComPtr<ID3D12Device> device);

		ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
		void SetCommandQueue(const ComPtr<ID3D12CommandQueue>& queue);

		std::vector<ComPtr<ID3D12GraphicsCommandList>> GetCommandLists() const;
		void SetCommandLists(const std::vector<ComPtr<ID3D12GraphicsCommandList>>& newCommandLists);

		std::vector<ComPtr<ID3D12CommandAllocator>> GetCommandAllocators() const;
		void SetCommandAllocators(const std::vector<ComPtr<ID3D12CommandAllocator>>& commandAllocators);

		std::vector<ComPtr<ID3D12Resource>>& GetRenderTargets();
		void SetRenderTargets(const std::vector<ComPtr<ID3D12Resource>>& renderTargets);

		ComPtr<ID3D12RootSignature> GetRootSignature() const;
		void SetRootSignature(const ComPtr<ID3D12RootSignature>& rootSignature);

		ComPtr<ID3D12PipelineState> GetPipelineState() const;
		void SetPipelineState(const ComPtr<ID3D12PipelineState>& pipelineState);

		void SetFences(std::vector<ComPtr<ID3D12Fence>> fences);
		std::vector<ComPtr<ID3D12Fence>>& GetFences();

		void SetFenceValues(std::vector<UINT64> fenceValues);
		std::vector<UINT64>& GetFenceValues();

		HANDLE GetFenceEvent() const;
		void SetFenceEvent(HANDLE event);

		UINT GetFrameIndex() const;
		void SetFrameIndex(UINT index);

		ComPtr<IDXGISwapChain3> GetSwapChain() const;
		void SetSwapChain(const ComPtr<IDXGISwapChain3>& swapChain);

		void SetBackBuffers(std::vector<ComPtr<ID3D12Resource>> backBuffers);
		std::vector<ComPtr<ID3D12Resource>>& GetBackBuffers();

		void SetConstantBuffer(const ComPtr<ID3D12Resource>& constantBuffer);
		ComPtr<ID3D12Resource> GetConstantBuffer() const;

		ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;
		void SetRtvHeap(const ComPtr<ID3D12DescriptorHeap>& rtvHeap);

		ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;
		void SetDsvHeap(const ComPtr<ID3D12DescriptorHeap>& dsvHeap);

		ComPtr<ID3D12DescriptorHeap> GetCbvHeap() const;
		void SetCbvHeap(const ComPtr<ID3D12DescriptorHeap>& cbvHeap);

		ComPtr<ID3D12Resource> GetDepthStencil() const;
		void SetDepthStencil(const ComPtr<ID3D12Resource>& depthStencil);

		void SetVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW& view);
		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;

		void SetIndexBufferView(const D3D12_INDEX_BUFFER_VIEW& view);
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

		void SetVertexBuffer(ComPtr<ID3D12Resource> vb);
		ComPtr<ID3D12Resource> GetVertexBuffer() const;

		void SetIndexBuffer(ComPtr<ID3D12Resource> ib);
		ComPtr<ID3D12Resource> GetIndexBuffer() const;

		D3D12_VIEWPORT GetViewport() const;
		void SetViewport(const D3D12_VIEWPORT& viewport);

		D3D12_RECT GetScissorRect() const;
		void SetScissorRect(const D3D12_RECT& scissorRect);

		float GetAspectRatio() const;
		void SetAspectRatio(float ratio);

		// engine systems here:
		
		//GameSceneSystem gameSceneSystem; // managment of game scenes
		//GameSystem internalGameSystem; // systems that are always prevalent (audio system, entity system)

	private:

		// window properties

		int frameRate;
		int totalFrames;
		int windowWidth;
		int windowHeight;
		std::wstring windowTitle;
		std::wstring windowClassName;

		// windows api handling

		HWND engineWindowHandle;

		static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// graphic properties

		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;
		std::vector<ComPtr<ID3D12GraphicsCommandList>> commandLists;
		std::vector<ComPtr<ID3D12CommandAllocator>> commandAllocators;
		std::vector<ComPtr<ID3D12Resource>> renderTargets;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12PipelineState> pipelineState;
		std::vector<ComPtr<ID3D12Fence>> fences;
		std::vector<UINT64> fenceValues;
		HANDLE fenceEvent;
		UINT frameIndex;
		UINT rtvDescriptorSize;
		ComPtr<IDXGISwapChain3> swapChain;
		std::vector<ComPtr<ID3D12Resource>> backBuffers;
		ComPtr<ID3D12Resource> m_constantBuffer;
		ComPtr<ID3D12DescriptorHeap> rtvHeap;
		ComPtr<ID3D12DescriptorHeap> dsvHeap;
		ComPtr<ID3D12DescriptorHeap> cbvHeap;
		ComPtr<ID3D12Resource> depthStencil;
		ComPtr<ID3D12Resource> vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		ComPtr<ID3D12Resource> indexBuffer;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;
		float aspectRatio;

		// resize handling

		void OnResize(UINT width, UINT height);

		// rendering

		void RenderFrame();
		void ResetCommandList(UINT currentFrameIndex);
		void draw(UINT currentFrameIndex);

	};

} // Engine

#endif // LEAF_ENGINE_H
