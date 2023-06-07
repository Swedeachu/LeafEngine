#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "Vector2D.h"
#include "..\stdafx.h"

namespace LeafMath
{

	class Matrix2D
	{

	public:

		// Constructors
		Matrix2D();
		Matrix2D(const Matrix2D& other);

		// Destructor
		~Matrix2D();

		// Assignment operator
		Matrix2D& operator=(const Matrix2D& other);

		// Set the matrix to the identity matrix
		void SetIdentity();

		// Get the element at the specified row and column
		float Get(int row, int col) const;

		// Transpose the matrix
		Matrix2D Transpose() const;

		// Concatenate two matrices
		Matrix2D Concat(const Matrix2D& other) const;

		// Translate the matrix by the specified x and y values
		void Translate(float x, float y);

		// Scale the matrix by the specified x and y values
		void Scale(float x, float y);

		// Rotate the matrix by the specified angle in degrees
		void RotateDeg(float angle);

		// Rotate the matrix by the specified angle in radians
		void RotateRad(float angle);

		// Multiply the matrix by a vector and return the resulting vector
		Vector2D MultiplyVector(const Vector2D& vec) const;

		// Get the matrix as a directX 4x4
		DirectX::XMFLOAT4X4 ToDirectXMatrix() const;

	private:
		float m[4][4];

	};

} // LeafMath

#endif // MATRIX2D_H
