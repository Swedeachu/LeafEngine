#include "Camera.h"
#include "..\EngineInstance.h"

namespace Graphics
{
	Camera::Camera(float x, float y, float zoom) : position(x, y), zoom(zoom)
	{}

	void Camera::SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	LeafMath::Vector2D Camera::GetPosition() const
	{
		return position;
	}

	void Camera::SetZoom(float zoom)
	{
		this->zoom = zoom;
	}

	float Camera::GetZoom() const
	{
		return zoom;
	}

	DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
	{
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixTranslation(-position.x, -position.y, 0));
		return viewMatrix;
	}

	DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
	{
		DirectX::XMFLOAT4X4 projectionMatrix;
		// Adjust the window width and height with zoom factor
		DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixOrthographicOffCenterLH(0, Engine::EngineInstance.GetWindowWidth() / zoom, Engine::EngineInstance.GetWindowHeight() / zoom, 0, 0.1f, 10.0f));
		return projectionMatrix;
	}

	// this implementation is completely wrong
	LeafMath::Vector2D Camera::ScreenPosToWorldPos(const LeafMath::Vector2D& screenPos)
	{
		// Calculate the scaled screen position based on the zoom level
		float scaledX = screenPos.x / zoom;
		float scaledY = screenPos.y / zoom;

		// Offset the scaled screen position by the camera position
		float worldX = position.x + scaledX;
		float worldY = position.y + scaledY;

		return LeafMath::Vector2D(worldX, worldY);
	}

}
