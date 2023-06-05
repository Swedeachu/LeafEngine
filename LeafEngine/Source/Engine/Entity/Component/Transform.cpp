#include "Transform.h"
#include <cmath>

namespace Component
{
	// Default constructor
	Transform::Transform() : position(0.0f, 0.0f), rotation(0.0f), scale(1.0f, 1.0f), isDirty(true), matrix() {}

	// Copy constructor
	Transform::Transform(const Transform& other) : position(other.position), rotation(other.rotation), scale(other.scale), isDirty(other.isDirty), matrix(other.matrix) {}

	// Constructor with initial values for position, rotation, and scale
	Transform::Transform(const LeafMath::Vector2D& position, float rotation, const LeafMath::Vector2D& scale) : position(position), rotation(rotation), scale(scale), isDirty(true) {}

	// Destructor
	Transform::~Transform()
	{}

	// Assignment operator
	Transform& Transform::operator=(const Transform& other)
	{
		if (this == &other)
		{
			return *this;
		}

		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		isDirty = other.isDirty;
		matrix = other.matrix;

		return *this;
	}

	// Get the transformation matrix
	const LeafMath::Matrix2D& Transform::GetMatrix() const
	{
		// Check if the matrix needs to be updated
		if (isDirty)
		{
			UpdateMatrix();
			isDirty = false;
		}

		return matrix;
	}

	// Get the position
	const LeafMath::Vector2D& Transform::GetPosition() const
	{
		return position;
	}

	// Get the rotation
	float Transform::GetRotation() const
	{
		return rotation;
	}

	// Get the scale
	const LeafMath::Vector2D& Transform::GetScale() const
	{
		return scale;
	}

	// Set the position
	void Transform::SetPosition(const LeafMath::Vector2D& translation)
	{
		this->position = translation;
		isDirty = true;
	}

	// Set the rotation
	void Transform::SetRotation(float rotation)
	{
		this->rotation = rotation;
		isDirty = true;
	}

	// Set the scale
	void Transform::SetScale(const LeafMath::Vector2D& scale)
	{
		this->scale = scale;
		isDirty = true;
	}

	// Update the transformation matrix
	void Transform::UpdateMatrix() const
	{
		LeafMath::Matrix2D translationMatrix;
		LeafMath::Matrix2D rotationMatrix;
		LeafMath::Matrix2D scaleMatrix;

		translationMatrix.Translate(position.x, position.y);
		rotationMatrix.RotateRad(rotation);
		scaleMatrix.Scale(scale.x, scale.y);

		matrix = rotationMatrix.Concat(scaleMatrix);
		matrix = translationMatrix.Concat(matrix);
	}

} // Component
