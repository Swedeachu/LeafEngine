#include "Matrix2D.h"
#include <cmath>

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

namespace LeafMath
{

	// Default constructor
	Matrix2D::Matrix2D()
	{
		// Initialize the matrix as the identity matrix
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}
	}

	// Copy constructor
	Matrix2D::Matrix2D(const Matrix2D& other)
	{
		// Copy the elements from the other matrix
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = other.m[i][j];
			}
		}
	}

	// Destructor
	Matrix2D::~Matrix2D() {}

	// Assignment operator
	Matrix2D& Matrix2D::operator=(const Matrix2D& other)
	{
		// Check for self-assignment
		if (this == &other)
		{
			return *this;
		}

		// Copy the elements from the other matrix
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = other.m[i][j];
			}
		}

		return *this;
	}

	// Set the matrix to the identity matrix
	void Matrix2D::SetIdentity()
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
				{
					m[i][j] = 1.0f;
				}
				else
				{
					m[i][j] = 0.0f;
				}
			}
		}
	}

	// Get the element at the specified row and column
	float Matrix2D::Get(int row, int col) const
	{
		if (row >= 0 && row < 4 && col >= 0 && col < 4)
		{
			return m[row][col];
		}
		else
		{
			return 0.0f; // Return a default value (you can modify it as needed)
		}
	}

	// Transpose the matrix
	Matrix2D Matrix2D::Transpose() const
	{
		Matrix2D result;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.m[j][i] = m[i][j];
			}
		}

		return result;
	}

	// Concatenate two matrices
	Matrix2D Matrix2D::Concat(const Matrix2D& other) const
	{
		Matrix2D result;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j] + m[i][3] * other.m[3][j];
			}
		}

		return result;
	}

	// Translate the matrix by the specified x and y values
	void Matrix2D::Translate(float x, float y)
	{
		Matrix2D translation;
		translation.SetIdentity();
		translation.m[0][3] = x;
		translation.m[1][3] = y;
		*this = Concat(translation);
	}

	// Scale the matrix by the specified x and y values
	void Matrix2D::Scale(float x, float y)
	{
		Matrix2D scale;
		scale.SetIdentity();
		scale.m[0][0] = x;
		scale.m[1][1] = y;
		*this = Concat(scale);
	}

	// Rotate the matrix by the specified angle in degrees
	void Matrix2D::RotateDeg(float angle)
	{
		float radians = angle * ((float)M_PI / 180.0f);
		Matrix2D rotation;
		rotation.SetIdentity();
		rotation.m[0][0] = cosf(radians);
		rotation.m[0][1] = -sinf(radians);
		rotation.m[1][0] = sinf(radians);
		rotation.m[1][1] = cosf(radians);
		*this = Concat(rotation);
	}

	// Rotate the matrix by the specified angle in radians
	void Matrix2D::RotateRad(float angle)
	{
		Matrix2D rotation;
		rotation.SetIdentity();
		rotation.m[0][0] = cosf(angle);
		rotation.m[0][1] = -sinf(angle);
		rotation.m[1][0] = sinf(angle);
		rotation.m[1][1] = cosf(angle);
		*this = Concat(rotation);
	}

	// Multiply the matrix by a vector and return the resulting vector
	Vector2D Matrix2D::MultiplyVector(const Vector2D& vec) const
	{
		Vector2D result;
		result.x = m[0][0] * vec.x + m[0][1] * vec.y;
		result.y = m[1][0] * vec.x + m[1][1] * vec.y;
		return result;
	}

	// Get the matrix as a directX 4x4
	DirectX::XMFLOAT4X4 Matrix2D::ToDirectXMatrix() const
	{
		DirectX::XMFLOAT4X4 directMatrix;

		directMatrix._11 = m[0][0];
		directMatrix._12 = m[0][1];
		directMatrix._13 = 0.0f;
		directMatrix._14 = 0.0f;

		directMatrix._21 = m[1][0];
		directMatrix._22 = m[1][1];
		directMatrix._23 = 0.0f;
		directMatrix._24 = 0.0f;

		directMatrix._31 = 0.0f;
		directMatrix._32 = 0.0f;
		directMatrix._33 = 1.0f;
		directMatrix._34 = 0.0f;

		directMatrix._41 = m[2][0];
		directMatrix._42 = m[2][1];
		directMatrix._43 = 0.0f;
		directMatrix._44 = 1.0f;

		return directMatrix;
	}

} // LeafMath
