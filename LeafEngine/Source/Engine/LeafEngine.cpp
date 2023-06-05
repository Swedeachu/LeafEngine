#include "stdafx.h"
#include "LeafEngine.h"
#include "Graphics/DirectWrapper.h"

namespace Engine
{

	// construct an instance of the leaf engine
	LeafEngine::LeafEngine(int fps, int width, int height, const std::wstring& title) : frameRate(fps), windowWidth(width), windowHeight(height), windowTitle(title)
	{
		totalFrames = 0;
		windowClassName = L"LeafEngineWindowClassDX";
		// Initialize the gameSceneSystem
		gameSceneSystem = GameSystem::GameSceneSystem();
	}

	// Release any resources
	LeafEngine::~LeafEngine()
	{
		directWrapper.Release();
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
		this->directWrapper.InitializeDirectX(engineWindowHandle, windowWidth, windowHeight);

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

		while (true)
		{
			// Handle window messages
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// Calculate delta time and total time
			QueryPerformanceCounter(&currTime);
			deltaTime = static_cast<double>(currTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
			prevTime = currTime;
			totalTime += deltaTime;

			// Check if it's time to update the screen
			if (totalTime >= frameTime)
			{
				// update the current frame
				gameSceneSystem.UpdateScene(frameDeltaTime);

				// render the current frame
				RenderFrame();

				// Update total amount of frames
				totalFrames++;

				// Subtract frame time from total time
				totalTime -= frameTime;
			}

			// Update frame delta time
			frameDeltaTime = static_cast<float>(deltaTime * 1000.0); // Convert to milliseconds
		}

		// remove all scenes which will effectively free all the entity lists
		gameSceneSystem.RemoveAllScenes();

		// delete ourselves once out of the main loop
		delete this;

		return static_cast<int>(msg.wParam);
	}

	// this function is called in the core loop for handling frames
	void LeafEngine::RenderFrame()
	{
		ID3D11DeviceContext* deviceContext = directWrapper.GetDeviceContext();

		// Clear the render target view
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		deviceContext->ClearRenderTargetView(directWrapper.GetRenderTargetView(), clearColor);

		// Render all current scene
		gameSceneSystem.RenderScene();

		// Present the rendered frame
		directWrapper.Present();
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

	float LeafEngine::GetFrameDeltaTime() const
	{
		return frameDeltaTime;
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

	GameSystem::GameSceneSystem& LeafEngine::GetGameSceneSystem()
	{
		return gameSceneSystem;
	}

} // Engine
