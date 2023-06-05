#include "Physics.h"

namespace Component
{

	// Default constructor
	Physics::Physics() : oldPosition(), acceleration(), velocity(), rotationalVelocity(0.0f) {}

	// Copy constructor
	Physics::Physics(const Physics& other) : oldPosition(other.oldPosition), acceleration(other.acceleration),
		velocity(other.velocity), rotationalVelocity(other.rotationalVelocity)
	{}

	// Constructor with initial values for acceleration, velocity, and rotational velocity
	Physics::Physics(const LeafMath::Vector2D& acceleration, const LeafMath::Vector2D& velocity, float rotationalVelocity) :
		acceleration(acceleration), velocity(velocity), rotationalVelocity(rotationalVelocity)
	{}

	// Destructor
	Physics::~Physics() {}

	// Assignment operator
	Physics& Physics::operator=(const Physics& other)
	{
		if (this == &other)
		{
			return *this;
		}

		oldPosition = other.oldPosition;
		acceleration = other.acceleration;
		velocity = other.velocity;
		rotationalVelocity = other.rotationalVelocity;

		return *this;
	}

	// Get the acceleration of the Physics component
	const LeafMath::Vector2D& Physics::GetAcceleration() const
	{
		return acceleration;
	}

	// Get the velocity of the Physics component
	const LeafMath::Vector2D& Physics::GetVelocity() const
	{
		return velocity;
	}

	// Get the rotational velocity of the Physics component
	float Physics::GetRotationalVelocity() const
	{
		return rotationalVelocity;
	}

	// Get the old position of the Physics component
	const LeafMath::Vector2D& Physics::GetOldPosition() const
	{
		return oldPosition;
	}

	// Set the acceleration of the Physics component
	void Physics::SetAcceleration(const LeafMath::Vector2D& acceleration)
	{
		this->acceleration = acceleration;
	}

	// Set the velocity of the Physics component
	void Physics::SetVelocity(const LeafMath::Vector2D& velocity)
	{
		this->velocity = velocity;
	}

	// Set the rotational velocity of the Physics component
	void Physics::SetRotationalVelocity(float rotationalVelocity)
	{
		this->rotationalVelocity = rotationalVelocity;
	}

	// Update the state of the Physics component
	void Physics::PhysicsUpdate(Transform& transform, float dt)
	{
		// Get the current position from the transform component
		const LeafMath::Vector2D& currentPosition = transform.GetPosition();

		// Store the current position as the old position in the Physics component
		oldPosition = currentPosition;

		// Calculate the new velocity using the acceleration and time
		// velocity = acceleration * dt + velocity
		LeafMath::Vector2D newVelocity = acceleration * dt + velocity;

		// Calculate the new position using the velocity and time
		// position = velocity * dt + position
		LeafMath::Vector2D newPosition = newVelocity * dt + currentPosition;

		// Set the new position in the transform component
		transform.SetPosition(newPosition);

		// Update the rotation based on the rotational velocity
		float currentRotation = transform.GetRotation();
		float newRotation = currentRotation + rotationalVelocity * dt;
		transform.SetRotation(newRotation);
	}

} // Component
