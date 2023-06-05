#ifndef ENTITY_H
#define ENTITY_H

#include "Component/Transform.h"
#include "Component/Physics.h"
#include "Component/Mesh.h"

using namespace Component;

namespace Entity
{

  class Entity
  {

  private:

    // components
    Transform transform;
    Physics physics;
    Mesh mesh;

    bool hasTransform;
    bool hasPhysics;
    bool hasMesh;

  public:

    // Constructor
    Entity();

    Entity(const Transform& newTransform, const Physics& newPhysics, const Mesh& newMesh);

    // Destructor
    ~Entity();

    // Get the transform component
    const Transform& GetTransform();

    // Set the transform component
    void SetTransform(const Transform& newTransform);

    // Get the physics component
    const Physics& GetPhysics();

    // Set the physics component
    void SetPhysics(const Physics& newPhysics);

    // Get the mesh component
    const Mesh& GetMesh();

    // Set the mesh component
    void SetMesh(const Mesh& newMesh);

    // Render the entity
    void EntityRender();

    // Update the entity
    void EntityUpdate(float deltaTime);

  };

} // Entity

#endif // ENTITY_H
