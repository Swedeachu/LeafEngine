#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <initializer_list>

namespace LeafMath
{

	class Vector2D
	{

	public:

		float x;
		float y;

		// Default constructor
		Vector2D();

		// Constructor with initial values
		Vector2D(float xVal, float yVal);

		// Set the coordinates of the vector
		void Set(float xVal, float yVal);

		// Get the length of the vector
		float Length() const;

		// Get the square of the length of the vector
		float SquareLength() const;

		// Normalize the vector to have a length of 1
		void Normalize();

		// Calculate the dot product of this vector with another vector
		float DotProduct(const Vector2D& other) const;

		// scales and adds two vectors
		Vector2D ScaleAdd(const Vector2D& vec0, const Vector2D& vec1, float scale);

		// scales and subs two vectors
		Vector2D ScaleSub(const Vector2D& vec0, const Vector2D& vec1, float scale);

		// scale a vector
		void Scale(float scale);

		// Calculate the distance between two vectors
		static float Distance(const Vector2D& vec1, const Vector2D& vec2);

		// Create a unit vector from an angle in degrees
		static Vector2D FromAngleDeg(float angle);

		// Create a unit vector from an angle in radians
		static Vector2D FromAngleRad(float angle);

		// Calculate the angle in radians of a given vector
		static float ToAngleRad(const Vector2D& vec);

		// Negation operator
		Vector2D operator-() const;

		// Addition operator
		Vector2D operator+(const Vector2D& other) const;

		// Subtraction operator
		Vector2D operator-(const Vector2D& other) const;

		// Scalar multiplication operator
		Vector2D operator*(float scale) const;

		// Scalar division operator
		Vector2D operator/(float scale) const;

		// set a vector like so, vec = { 0, 0 }
		Vector2D& operator=(const std::initializer_list<float>& values);

	};

} // LeafMath

#endif // VECTOR2D_H
