#ifndef LEAF_ENGINE_H
#define LEAF_ENGINE_H

#include "stdafx.h"
#include "Graphics/DirectWrapper.h"  
#include "Graphics/MeshLibrary.h"
#include "Graphics/Camera.h"
#include "System/GameScene/GameSceneSystem.h"

namespace Engine
{

	class LeafEngine
	{

	public:

		// default constructor that defaults as 60, 300, 300, L"Leaf Engine Game"
		LeafEngine();

		// create the leaf engine and its window
		LeafEngine(int fps, int width, int height, const std::wstring& title);

		// deconstructor should free and release any engine any systems and data structures
		~LeafEngine();

		// run the engine
		int Run();

		// Getter and Setter for frame rate
		int GetFrameRate() const;
		void SetFrameRate(const int fps);
		int GetTotalFrames() const;
		float GetFrameDeltaTime() const;

		// Getter and Setter for window size
		int GetWindowWidth() const;
		int GetWindowHeight() const;
		void SetWindowSize(const int width, const int height);

		// Getters for systems
		Graphics::DirectWrapper& GetDirectWrapper();
		Graphics::MeshLibrary& GetMeshLibrary();
		Graphics::Camera& GetCamera();
		GameSystem::GameSceneSystem& GetGameSceneSystem();

		// Get the window handle for the engine's instance
		HWND GetEngineWindowHandle() const;

		// Getter for the window title
		const std::wstring& GetWindowTitle() const;

		// Setter for the window title
		void SetWindowTitle(const std::wstring& title);

	private:

		// simply sets the totalFrames and class name and creates the system objects
		void ShallowInit();

		// window properties
		int frameRate;
		float frameDeltaTime;
		int totalFrames;
		int windowWidth;
		int windowHeight;
		std::wstring windowTitle;
		std::wstring windowClassName;

		// windows api handling
		HWND engineWindowHandle;

		static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Systems and required components
		Graphics::DirectWrapper directWrapper;
		Graphics::MeshLibrary meshLibrary;
		Graphics::Camera camera;
		GameSystem::GameSceneSystem gameSceneSystem;

		// rendering/frame update
		void RenderFrame();

	};

} // Engine

#endif // LEAF_ENGINE_H
