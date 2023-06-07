#ifndef MESH_H
#define MESH_H

#include "DirectWrapper.h"

namespace Graphics
{

  struct Vertex
  {
    DirectX::XMFLOAT2 position;
    DirectX::XMFLOAT4 color;
    DirectX::XMFLOAT2 texCoord;
  };

  class Mesh
  {

  public:

    Mesh();
    ~Mesh();

    void Initialize(Graphics::DirectWrapper& directWrapper, const Vertex* vertices, UINT vertexCount);
    void Draw(Graphics::DirectWrapper& directWrapper);

    static Mesh CreateQuad(Graphics::DirectWrapper& directWrapper, const DirectX::XMFLOAT2& size);

  private:

    ID3D11Buffer* vertexBuffer;
    UINT vertexCount;
    UINT stride;
    UINT offset;

  };

} // Graphics

#endif // MESH_H
