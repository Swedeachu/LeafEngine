#include "Transform.h"
#include <cmath>

namespace Component
{

	// Default constructor
	Transform::Transform() : position(LeafMath::Vector2D()), scale(LeafMath::Vector2D(1.0f, 1.0f)), rotation(0.0f)
	{
		// Set the initial transformation matrix to the identity matrix
		matrix.SetIdentity();
	}

	// Constructor with initial position, scale, and rotation
	Transform::Transform(const LeafMath::Vector2D& position, const LeafMath::Vector2D& scale, float rotation) : position(position), scale(scale), rotation(rotation)
	{
		// Set the initial transformation matrix based on the position, scale, and rotation
		matrix.SetIdentity();
		matrix.Translate(position.x, position.y);
		matrix.RotateRad(rotation);
		matrix.Scale(scale.x, scale.y);
	}

	// deconstructor
	Transform::~Transform()
	{
		// Call the destructor of the matrix object
		matrix.~Matrix2D();
	}

	// Get the position
	const LeafMath::Vector2D& Transform::GetPosition() const
	{
		return position;
	}

	// Set the position
	void Transform::SetPosition(const LeafMath::Vector2D& position)
	{
		this->position = position;
		// Update the transformation matrix
		matrix.SetIdentity();
		matrix.Translate(position.x, position.y);
		matrix.RotateRad(rotation);
		matrix.Scale(scale.x, scale.y);
	}

	// Get the scale
	const LeafMath::Vector2D& Transform::GetScale() const
	{
		return scale;
	}

	// Set the scale
	void Transform::SetScale(const LeafMath::Vector2D& scale)
	{
		this->scale = scale;
		// Update the transformation matrix
		matrix.SetIdentity();
		matrix.Translate(position.x, position.y);
		matrix.RotateRad(rotation);
		matrix.Scale(scale.x, scale.y);
	}

	// Get the rotation
	float Transform::GetRotation() const
	{
		return rotation;
	}

	// Set the rotation
	void Transform::SetRotation(float rotation)
	{
		this->rotation = rotation;
		// Update the transformation matrix
		matrix.SetIdentity();
		matrix.Translate(position.x, position.y);
		matrix.RotateRad(rotation);
		matrix.Scale(scale.x, scale.y);
	}

	// Get the transformation matrix
	const LeafMath::Matrix2D& Transform::GetMatrix() const
	{
		return matrix;
	}

	// Set the transformation matrix
	void Transform::SetMatrix(const LeafMath::Matrix2D& matrix)
	{
		this->matrix = matrix;
		// Update the position, scale, and rotation based on the new matrix
		position = LeafMath::Vector2D(matrix.Get(0, 3), matrix.Get(1, 3));
		scale = LeafMath::Vector2D(matrix.Get(0, 0), matrix.Get(1, 1));
		rotation = std::atan2(matrix.Get(1, 0), matrix.Get(0, 0));
	}

} // namespace Component
