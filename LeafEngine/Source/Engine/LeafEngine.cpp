#include "stdafx.h"
#include "LeafEngine.h"
#include "Graphics/DirectWrapper.h"

namespace Engine
{

	// default constructor that defaults as 60, 300, 300, L"default"
	LeafEngine::LeafEngine() : frameRate(60), windowWidth(300), windowHeight(300), windowTitle(L"Leaf Engine Game")
	{
		resizing = false;
		ShallowInit();
	}

	// construct an instance of the leaf engine with a set fps, window width and height, and a title
	LeafEngine::LeafEngine(int fps, int width, int height, const std::wstring& title) : frameRate(fps), windowWidth(width), windowHeight(height), windowTitle(title)
	{
		ShallowInit();
	}

	// simply sets the totalFrames and class name and creates the system objects
	void LeafEngine::ShallowInit()
	{
		totalFrames = 0;
		windowClassName = L"LeafEngineWindowClassDX";
		// Create the systems
		gameSceneSystem = GameSystem::GameSceneSystem();
		meshLibrary = Graphics::MeshLibrary();
		inputHandler = Input::InputHandler();
	}

	// Release any resources
	LeafEngine::~LeafEngine()
	{
		// remove all scenes which will effectively free all the entity lists
		gameSceneSystem.RemoveAllScenes();
		// release the directX wrapper and its objects
		directWrapper.ReleaseDirectX();
	}

	// stupid hack around to get the windows callback function we want for setting the windows class, 
	// as well as getting the window to know it was created by LeafEngine
	LRESULT CALLBACK LeafEngine::StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Retrieve the pointer to the LeafEngine instance
		LeafEngine* enginePtr = reinterpret_cast<LeafEngine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if (uMsg == WM_CREATE)
		{
			// Set the GWLP_USERDATA value to the LeafEngine instance when the window is created
			CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			enginePtr = reinterpret_cast<LeafEngine*>(createStruct->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(enginePtr));
		}

		// Call the non-static member function for window procedure
		return enginePtr->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	// basic message reciever
	LRESULT CALLBACK LeafEngine::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			// destroy handling ---------------------------------------------------------------
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
				// move handling ----------------------------------------------------------------
			case WM_MOVE:
				InvalidateRect(hwnd, NULL, FALSE); // force redraw
				break;
				// size handling ----------------------------------------------------------------
			case WM_SIZE:
				if (wParam == SIZE_MAXIMIZED)
				{
					camera.ResetWindowSize();
				}
				if (wParam == SIZE_RESTORED && !resizing)
				{
					camera.ResetWindowSize();
				}
				break;
			case WM_SIZING:
				resizing = true;
				break;
			case WM_EXITSIZEMOVE:
				if (resizing)
				{
					camera.ResetWindowSize();
				}
				resizing = false;
				break;
				// Input handling from here ------------------------------------------------------
			case WM_KEYDOWN:
				inputHandler.KeySetState((unsigned char)wParam, true);
				break;
			case WM_KEYUP:
				inputHandler.KeySetState((unsigned char)wParam, false);
				break;
			case WM_SYSKEYDOWN:
				if (wParam == VK_F10)
				{
					inputHandler.KeySetState(VK_F10, true);
				}
				break;
			case WM_SYSKEYUP:
				if (wParam == VK_F10)
				{
					inputHandler.KeySetState(VK_F10, false);
				}
				break;
			case WM_MOUSEWHEEL:
				inputHandler.SetMouseScrollDelta(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
				break;
			case WM_LBUTTONDOWN:
				inputHandler.KeySetState(VK_LBUTTON, true);
				break;
			case WM_LBUTTONUP:
				inputHandler.KeySetState(VK_LBUTTON, false);
				break;
			case WM_RBUTTONDOWN:
				inputHandler.KeySetState(VK_RBUTTON, true);
				break;
			case WM_RBUTTONUP:
				inputHandler.KeySetState(VK_RBUTTON, false);
				break;
			case WM_MBUTTONDOWN:
				inputHandler.KeySetState(VK_MBUTTON, true);
				break;
			case WM_MBUTTONUP:
				inputHandler.KeySetState(VK_MBUTTON, false);
				break;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

#pragma region Input Handling Wrapper

	// must be upper cased when using chars!!!
	bool LeafEngine::InputKeyDown(char key)
	{
		return inputHandler.IsKeyDown(key);
	}

	// must be upper cased when using chars!!!
	bool LeafEngine::InputKeyTriggered(char key)
	{
		return inputHandler.IsKeyTriggered(key);
	}

	// must be upper cased when using chars!!!
	bool LeafEngine::InputKeyReleased(char key)
	{
		return inputHandler.IsKeyReleased(key);
	}

	const LeafMath::Vector2D& LeafEngine::GetMousePosition() const
	{
		return inputHandler.GetMousePosition();
	}

	const LeafMath::Vector2D& LeafEngine::GetMousePositionDelta() const
	{
		return inputHandler.GetMousePositionDelta();
	}

	void LeafEngine::ShowMouseCursor(bool show)
	{
		int count = show ? 1 : -1;
		while (count != 0)
		{
			count = show ? ShowCursor(TRUE) : ShowCursor(FALSE);
		}
	}

#pragma endregion End input handling

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
			this // Set the GWLP_USERDATA to the LeafEngine instance
		);

		if (engineWindowHandle == nullptr)
		{
			return 1; // error if the handle isn't made correctly
		}

		// first set up the camera
		camera.InitCamera(engineWindowHandle);
		// Set up the DirectX device and other needed things on the LeafEngine instance now that we are actually started
		this->directWrapper.InitializeDirectX(engineWindowHandle, windowWidth, windowHeight);
		// then we can init the mesh library
		meshLibrary.InitMeshLibrary();

		// Show and update the window
		ShowWindow(engineWindowHandle, SW_SHOW);
		UpdateWindow(engineWindowHandle);

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
			// read inputs
			inputHandler.InputUpdate();

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

		// delete ourselves once out of the main loop
		delete this;

		return static_cast<int>(msg.wParam);
	}

	// this function is called in the core loop for handling frames
	void LeafEngine::RenderFrame()
	{
		// Begin current frame
		directWrapper.BeginFrame();

		// Render current scene
		gameSceneSystem.RenderScene();

		// End current frame
		directWrapper.EndFrame();
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

	// check if the window is currently focused
	bool LeafEngine::IsActiveWindow() const
	{
		return GetActiveWindow() == engineWindowHandle;
	}

	// Getter for the window title
	const std::wstring& LeafEngine::GetWindowTitle() const
	{
		return windowTitle;
	}

	// Setter for the window title
	void LeafEngine::SetWindowTitle(const std::wstring& title)
	{
		windowTitle = title;
		if (engineWindowHandle != nullptr)
		{
			SetWindowText(engineWindowHandle, windowTitle.c_str());
		}
	}

#pragma endregion end of window properties

#pragma region Getters for engine systems and major components

	Graphics::DirectWrapper& LeafEngine::GetDirectWrapper()
	{
		return directWrapper;
	}

	Graphics::MeshLibrary& LeafEngine::GetMeshLibrary()
	{
		return meshLibrary;
	}

	Graphics::Camera& LeafEngine::GetCamera()
	{
		return camera;
	}

	GameSystem::GameSceneSystem& LeafEngine::GetGameSceneSystem()
	{
		return gameSceneSystem;
	}

#pragma endregion End of getters for engine systems and major components

} // Engine
