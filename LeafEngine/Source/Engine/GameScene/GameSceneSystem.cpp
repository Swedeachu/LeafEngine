#include "../stdafx.h"

#include "GameSceneSystem.h"

namespace Engine
{

	GameSceneSystem::GameSceneSystem() : currentScene(nullptr) {}

	void GameSceneSystem::SetGameScene(const GameScene& scene)
	{
		currentScene = scene;
		currentScene.Start();
	}

	void GameSceneSystem::StartGameScene()
	{
		currentScene.Start();
	}

	void GameSceneSystem::RestartCurrentScene()
	{
		currentScene.Restart();
	}

	void GameSceneSystem::UpdateCurrentScene()
	{
		currentScene.Update();
	}

	void GameSceneSystem::ExitCurrentScene()
	{
		currentScene.Exit();
	}

	void GameSceneSystem::RenderCycle(ID3D12Device2* device, ID3D12GraphicsCommandList* commandList,
		ID3D12CommandQueue* commandQueue, IDXGISwapChain3* swapChain,
		ID3D12Resource** renderTargets, ID3D12CommandAllocator* commandAllocator,
		ID3D12PipelineState* pipelineState, ID3D12DescriptorHeap* rtvHeap,
		UINT frameIndex, UINT rtvDescriptorSize, ID3D12Fence* fence, UINT64 fenceValue)
	{
		// Begin DirectX 12 drawing operations
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, pipelineState, IID_PPV_ARGS(&commandList));

		// Create a barrier for transitioning resource states
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = renderTargets[frameIndex];
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrier);

		// Create a descriptor handle for the render target view
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		rtvHandle.ptr = rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr + frameIndex * rtvDescriptorSize;
		commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		// Update and render all objects in the current scene
		UpdateCurrentScene();

		// End DirectX 12 drawing operations
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		commandList->ResourceBarrier(1, &barrier);

		// Execute CommandList
		commandList->Close();
		ID3D12CommandList* ppCommandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame
		swapChain->Present(1, 0);

		// Wait for the frame to finish
		HANDLE hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		commandQueue->Signal(fence, fenceValue);
		fence->SetEventOnCompletion(fenceValue, hEvent);
		if (hEvent)
		{
			WaitForSingleObject(hEvent, INFINITE);
			CloseHandle(hEvent);
		}
	}

} // Engine
