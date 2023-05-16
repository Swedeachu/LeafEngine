#ifndef GAME_SCENE_SYSTEM_H
#define GAME_SCENE_SYSTEM_H

#include "GameScene.h"

namespace Engine
{

	class GameSceneSystem
	{

	public:

		GameSceneSystem();

		void SetGameScene(const GameScene& scene);

		void StartGameScene();
		void RestartCurrentScene();
		void UpdateCurrentScene();
		void ExitCurrentScene();

		void RenderCycle(ID3D12Device2* device,
			ID3D12GraphicsCommandList* commandList,
			ID3D12CommandQueue* commandQueue,
			IDXGISwapChain3* swapChain,
			ID3D12Resource** renderTargets,
			ID3D12CommandAllocator* commandAllocator,
			ID3D12PipelineState* pipelineState,
			ID3D12DescriptorHeap* rtvHeap,
			UINT frameIndex,
			UINT rtvDescriptorSize,
			ID3D12Fence* fence,
			UINT64 fenceValue);

	private:

		GameScene currentScene;

	};

} // Engine

#endif // GAME_SCENE_SYSTEM_H
