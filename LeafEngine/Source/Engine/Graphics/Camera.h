#ifndef CAMERA_H
#define CAMERA_H

#include "..\stdafx.h"
#include "..\LeafMath\Vector2D.h"

namespace Graphics
{

	class Camera
	{

	public:

		void InitCamera(HWND hwnd);

		void SetPosition(float x, float y);
		LeafMath::Vector2D GetPosition() const;

		float GetZoom() const;
		void SetZoom(float zoom);

		void ResetWindowSize();

		DirectX::XMFLOAT4X4 GetWorldMatrix();

		LeafMath::Vector2D ScreenPosToWorldPos(const LeafMath::Vector2D& screenPos);

	private:

		LeafMath::Vector2D position{ 0.0f, 0.0f };
		LeafMath::Vector2D windowSize{ 0.0f, 0.0f };

		float cameraZ{ 0.0f };
		float scale{ 0.0f };

		DirectX::XMMATRIX viewProjectionMatrix{
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
		};;

		HWND sharedWindowHandle{ nullptr };

	};

} // Graphics

#endif // CAMERA_H
