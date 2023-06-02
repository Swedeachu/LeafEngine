#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "..\..\LeafMath\Vector2D.h"
#include "..\..\LeafMath\Matrix2D.h"

namespace Component
{
	class Transform
	{
	private:
		LeafMath::Vector2D position;  // The translation (or world position) of an object.
		LeafMath::Vector2D scale;     // The scale of a Game Object by width and height, should be 1 by 1 default (for example: 32 by 32 sprite).
		LeafMath::Matrix2D matrix;    // The transformation matrix.
		float rotation;               // The rotation (or orientation) of an object (in degrees).

	public:
			// Default constructor
		Transform();

		// Constructor with initial position, scale, and rotation
		Transform(const LeafMath::Vector2D& position, const LeafMath::Vector2D& scale, float rotation);

		// deconstructor
		~Transform();

		// Get the position
		const LeafMath::Vector2D& GetPosition() const;

		// Set the position
		void SetPosition(const LeafMath::Vector2D& position);

		// Get the scale
		const LeafMath::Vector2D& GetScale() const;

		// Set the scale
		void SetScale(const LeafMath::Vector2D& scale);

		// Get the rotation
		float GetRotation() const;

		// Set the rotation
		void SetRotation(float rotation);

		// Get the transformation matrix
		const LeafMath::Matrix2D& GetMatrix() const;

		// Set the transformation matrix
		void SetMatrix(const LeafMath::Matrix2D& matrix);
	};

} // Component

#endif // TRANSFORM_H
