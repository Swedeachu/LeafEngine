#ifndef CAMERA_H
#define CAMERA_H

#include "DirectXMath.h"
#include "..\LeafMath\Vector2D.h"

namespace Graphics
{

  class Camera
  {

  public:

    Camera(float x = 0.0f, float y = 0.0f, float zoom = 1.0f);

    void SetPosition(float x, float y);
    LeafMath::Vector2D GetPosition() const;

    void SetZoom(float zoom);
    float GetZoom() const;

    DirectX::XMFLOAT4X4 GetViewMatrix();
    DirectX::XMFLOAT4X4 GetProjectionMatrix();

    LeafMath::Vector2D ScreenPosToWorldPos(const LeafMath::Vector2D& screenPos);

  private:

    LeafMath::Vector2D position;
    float zoom;

  };

} // Graphics

#endif // CAMERA_H
