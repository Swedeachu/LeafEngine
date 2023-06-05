#ifndef PHYSICS_H
#define PHYSICS_H

#include "..\..\LeafMath\Vector2D.h"
#include "Transform.h"

namespace Component
{

	// The Physics class represents the physics properties of an object in 2D space
	class Physics
	{

	public:

		// Default constructor
		Physics();

		// Copy constructor
		Physics(const Physics& other);

		// Constructor with initial values
		Physics(const LeafMath::Vector2D& acceleration, const LeafMath::Vector2D& velocity, float rotationalVelocity);

		// Destructor
		~Physics();

		// Assignment operator
		Physics& operator=(const Physics& other);

		// Get the acceleration
		const LeafMath::Vector2D& GetAcceleration() const;

		// Get the velocity
		const LeafMath::Vector2D& GetVelocity() const;

		// Get the rotational velocity
		float GetRotationalVelocity() const;

		// Get the old position
		const LeafMath::Vector2D& GetOldPosition() const;

		// Set the acceleration
		void SetAcceleration(const LeafMath::Vector2D& acceleration);

		// Set the velocity
		void SetVelocity(const LeafMath::Vector2D& velocity);

		// Set the rotational velocity
		void SetRotationalVelocity(float rotationalVelocity);

		// Update the physics state based on the given transform and time increment
		void PhysicsUpdate(Transform& transform, float dt);

	private:

		LeafMath::Vector2D oldPosition;
		LeafMath::Vector2D acceleration;
		LeafMath::Vector2D velocity;
		float rotationalVelocity;

	};

} // Component

#endif // PHYSICS_H
