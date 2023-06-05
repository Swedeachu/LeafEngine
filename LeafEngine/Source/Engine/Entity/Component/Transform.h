#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "..\..\LeafMath\Vector2D.h"
#include "..\..\LeafMath\Matrix2D.h"

namespace Component
{

	// The Transform class represents a transformation in 2D space
	class Transform
	{

	public:

		// Default constructor
		Transform();

		// Copy constructor
		Transform(const Transform& other);

		// Constructor with initial values
		Transform(const LeafMath::Vector2D& position, float rotation, const LeafMath::Vector2D& scale);

		// Destructor
		~Transform();

		// Assignment operator
		Transform& operator=(const Transform& other);

		// Get the transformation matrix
		const LeafMath::Matrix2D& GetMatrix() const;

		// Get the position
		const LeafMath::Vector2D& GetPosition() const;

		// Get the rotation
		float GetRotation() const;

		// Get the scale
		const LeafMath::Vector2D& GetScale() const;

		// Set the position
		void SetPosition(const LeafMath::Vector2D& position);

		// Set the rotation
		void SetRotation(float rotation);

		// Set the scale
		void SetScale(const LeafMath::Vector2D& scale);

	private:

		// Update the transformation matrix
		void UpdateMatrix() const;

		LeafMath::Vector2D position;    // Position of the transform
		float rotation;                 // Rotation angle of the transform
		LeafMath::Vector2D scale;       // Scale of the transform
		mutable bool isDirty;           // Flag indicating if the matrix needs to be updated
		mutable LeafMath::Matrix2D matrix;  // Transformation matrix

	};

} // Component

#endif // TRANSFORM_H
