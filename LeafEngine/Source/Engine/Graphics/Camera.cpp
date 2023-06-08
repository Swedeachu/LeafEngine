#include "Camera.h"
#include "..\EngineInstance.h"

namespace Graphics
{

	void Camera::InitCamera(HWND hwnd)
	{
		position.x = 0.0f;
		position.y = 0.0f;
		cameraZ = -10.0f;
		scale = 1.0f;
		viewProjectionMatrix = DirectX::XMMatrixIdentity();
		sharedWindowHandle = hwnd;
		// Get the window size 
		RECT rect;
		GetClientRect(hwnd, &rect);
		// Save the window size
		windowSize = { (float)(rect.right - rect.left), (float)(rect.bottom - rect.top) };
	}

	void Camera::SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	LeafMath::Vector2D Camera::GetPosition() const
	{
		return position;
	}

	float Camera::GetZoom() const
	{
		return scale;
	}

	void Camera::SetZoom(float zoom)
	{
		// Set the new zoom factor
		scale = zoom;
		// Update the world matrix on the constant buffer
		Engine::EngineInstance.GetDirectWrapper().SetWorldViewMatrix(GetWorldMatrix());
	}

	void Camera::ResetWindowSize()
	{
		if (sharedWindowHandle)
		{
			// Get the window size 
			RECT rect;
			GetClientRect(sharedWindowHandle, &rect);
			// Save the window size
			windowSize = { (float)(rect.right - rect.left), (float)(rect.bottom - rect.top) };
			if (windowSize.x <= 0.0f || windowSize.y <= 0.0f)
			{
				return;
			}
			// Tell the graphics system to reset for the new window size
			Engine::EngineInstance.GetDirectWrapper().HandleWindowResize(sharedWindowHandle);
		}
	}

	// converts a xm matrix to a xm float 4x4 matrix
	static DirectX::XMFLOAT4X4 ConvertXM4x4(const DirectX::XMMATRIX& matrix)
	{
		DirectX::XMFLOAT4X4 newMatrix;
		DirectX::XMFLOAT4X4 temp;
		DirectX::XMStoreFloat4x4(&temp, matrix);
		memcpy(newMatrix.m, temp.m, sizeof(float) * 16);
		return newMatrix;
	}

	DirectX::XMFLOAT4X4 Camera::GetWorldMatrix()
	{
		DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(position.x, position.y, cameraZ, 0.0f),
			DirectX::XMVectorSet(position.x, position.y, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);

		DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicLH(
			windowSize.x * scale,
			windowSize.y * scale,
			1.0f,
			1000.0f
		);

		viewProjectionMatrix = viewMatrix * projectionMatrix;

		return ConvertXM4x4(DirectX::XMMatrixTranspose(viewProjectionMatrix));
	}

	LeafMath::Vector2D Camera::ScreenPosToWorldPos(const LeafMath::Vector2D& screenPos)
	{
		DirectX::XMVECTOR temp;
		DirectX::XMMATRIX invViewProj = DirectX::XMMatrixInverse(&temp, viewProjectionMatrix);
		DirectX::XMVECTOR adjPosVec{
			 (((2.0f * screenPos.x) / windowSize.x) - 1),
			 -(((2.0f * screenPos.y) / windowSize.y) - 1),
			 1.0, 0.0f
		};
		DirectX::XMVECTOR transformedPos = DirectX::XMVector4Transform(adjPosVec, invViewProj);
		return { DirectX::XMVectorGetX(transformedPos) + position.x, DirectX::XMVectorGetY(transformedPos) + position.y };
	}

}
