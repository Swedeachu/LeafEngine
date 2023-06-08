#include "InputHandler.h"

#include "..\EngineInstance.h"

namespace Input
{

	void InputHandler::InputUpdate()
	{
		// Set past frames key states
		for (auto& key : keyState)
		{
			key.second.second = key.second.first;
		}

		// If we aren't the active window, don't have any keys set to true
		if (!Engine::EngineInstance.IsActiveWindow())
		{
			for (auto& key : keyState)
			{
				key.second.first = false;
			}
		}

		// Update the mouse position
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		ScreenToClient(Engine::EngineInstance.GetEngineWindowHandle(), &mousePoint);

		// Save mouse delta
		mouseDelta.x = mousePoint.x - mousePos.x;
		mouseDelta.y = mousePoint.y - mousePos.y;

		// Update mouse position 
		mousePos = { static_cast<float>(mousePoint.x), static_cast<float>(mousePoint.y) };

		// Reset mouse wheel delta
		mouseWheelDelta = 0;
	}

	// Set the state of the key if it is down or not
	void InputHandler::KeySetState(unsigned char key, bool isDown)
	{
		if (key < keyCount && key > 0)
		{
			keyState[key].second.first = isDown;
		}
	}

	// Check if the key is currently down
	bool InputHandler::IsKeyDown(unsigned char key) const
	{
		if (key < keyCount && key > 0)
		{
			return keyState[key].second.first;
		}
		return false;
	}

	// Check if the key was just triggered this frame
	bool InputHandler::IsKeyTriggered(unsigned char key) const
	{
		if (key < keyCount && key > 0)
		{
			return keyState[key].second.first && !keyState[key].second.second;
		}
		return false;
	}

	// Check if the key was just released this frame
	bool InputHandler::IsKeyReleased(unsigned char key) const
	{
		if (key < keyCount && key > 0)
		{
			return !keyState[key].second.first && keyState[key].second.second;
		}
		return false;
	}

	void InputHandler::SetMouseScrollDelta(int delta)
	{
		mouseWheelDelta = delta;
	}

	int InputHandler::GetMouseScrollDelta()
	{
		return mouseWheelDelta;
	}

	const LeafMath::Vector2D& InputHandler::GetMousePosition() const
	{
		return mousePos;
	}

	const LeafMath::Vector2D& InputHandler::GetMousePositionDelta() const
	{
		return mouseDelta;
	}

} // Input
