#include "Vector2D.h"
#include <cmath>

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

using namespace std;

namespace LeafMath
{

	Vector2D::Vector2D() : x(0.0f), y(0.0f) {}

	Vector2D::Vector2D(float xVal, float yVal) : x(xVal), y(yVal) {}

	void Vector2D::Set(float xVal, float yVal)
	{
		x = xVal;
		y = yVal;
	}

	float Vector2D::Length() const
	{
		return sqrt(x * x + y * y);
	}

	float Vector2D::SquareLength() const
	{
		return x * x + y * y;
	}

	void Vector2D::Normalize()
	{
		float length = Length();
		if (length != 0.0f)
		{
			x /= length;
			y /= length;
		}
	}

	float Vector2D::DotProduct(const Vector2D& other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2D Vector2D::ScaleAdd(const Vector2D& vec0, const Vector2D& vec1, float scale)
	{
		return Vector2D((vec0.x * scale) + vec1.x, (vec0.y * scale) + vec1.y);
	}

	Vector2D Vector2D::ScaleSub(const Vector2D& vec0, const Vector2D& vec1, float scale)
	{
		return Vector2D((vec0.x * scale) - vec1.x, (vec0.y * scale) - vec1.y);
	}

	void Vector2D::Scale(float scale)
	{
		x *= scale;
		y *= scale;
	}

	float Vector2D::Distance(const Vector2D& vec1, const Vector2D& vec2)
	{
		float dx = vec2.x - vec1.x;
		float dy = vec2.y - vec1.y;
		return sqrt(dx * dx + dy * dy);
	}

	Vector2D Vector2D::FromAngleDeg(float angle)
	{
		float radians = angle * ((float)M_PI / 180.0f);
		return Vector2D(cos(radians), sin(radians));
	}

	Vector2D Vector2D::FromAngleRad(float angle)
	{
		return Vector2D(cos(angle), sin(angle));
	}

	float Vector2D::ToAngleRad(const Vector2D& vec)
	{
		return atan2(vec.y, vec.x);
	}

	Vector2D Vector2D::operator-() const
	{
		return Vector2D(-x, -y);
	}

	Vector2D Vector2D::operator+(const Vector2D& other) const
	{
		return Vector2D(x + other.x, y + other.y);
	}

	Vector2D Vector2D::operator-(const Vector2D& other) const
	{
		return Vector2D(x - other.x, y - other.y);
	}

	Vector2D Vector2D::operator*(float scale) const
	{
		return Vector2D(x * scale, y * scale);
	}

	Vector2D Vector2D::operator/(float scale) const
	{
		if (scale != 0.0f)
		{
			float invScale = 1.0f / scale;
			return Vector2D(x * invScale, y * invScale);
		}
		return Vector2D();
	}

	Vector2D& Vector2D::operator=(const initializer_list<float>& values)
	{
		if (values.size() >= 2)
		{
			auto it = values.begin();
			x = *it++;
			y = *it;
		}
		return *this;
	}

} // LeafMath
