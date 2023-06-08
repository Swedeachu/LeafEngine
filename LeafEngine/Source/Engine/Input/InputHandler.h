#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <array>
#include <Windows.h>
#include "..\LeafMath\Vector2D.h"

namespace Input
{

	class InputHandler
	{

	public:

		void InputUpdate();

		void KeySetState(unsigned char key, bool isDown);

		bool IsKeyDown(unsigned char key) const;

		bool IsKeyTriggered(unsigned char key) const;

		bool IsKeyReleased(unsigned char key) const;

		void SetMouseScrollDelta(int delta);

		int GetMouseScrollDelta();

		const LeafMath::Vector2D& GetMousePosition() const;

		const LeafMath::Vector2D& GetMousePositionDelta() const;

	private:

		// amount of valid keys to accept
		static constexpr int keyCount{ 256 };

		// 256 size array of all keys with a pair of booleans, 
		// first boolean holds the state of the key for the current frame
		// second boolean holds the state of the key for the previous frame
		// array[char key -> <bool currentState, bool previousState>
		std::array<std::pair<char, std::pair<bool, bool>>, keyCount> keyState;

		int mouseWheelDelta{ 0 };
		LeafMath::Vector2D mousePos{ 0.0f, 0.0f };
		LeafMath::Vector2D mouseDelta{ 0.0f, 0.0f };

	};

} // Input

#endif // INPUT_HANDLER_H
