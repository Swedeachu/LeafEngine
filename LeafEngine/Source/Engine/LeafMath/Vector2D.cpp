#include "Vector2D.h"
#include <cmath>

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

using namespace std;

namespace LeafMath
{

  // Default constructor
  Vector2D::Vector2D() : x(0.0f), y(0.0f) {}

  // Constructor with initial values
  Vector2D::Vector2D(float xVal, float yVal) : x(xVal), y(yVal) {}

  // Set the vector components
  void Vector2D::Set(float xVal, float yVal)
  {
    x = xVal;
    y = yVal;
  }

  // Calculate the length of the vector
  float Vector2D::Length() const
  {
    return sqrt(x * x + y * y);
  }

  // Calculate the squared length of the vector
  float Vector2D::SquareLength() const
  {
    return x * x + y * y;
  }

  // Normalize the vector
  void Vector2D::Normalize()
  {
    float length = Length();
    if (length != 0.0f)
    {
      x /= length;
      y /= length;
    }
  }

  // Calculate the dot product between two vectors
  float Vector2D::DotProduct(const Vector2D& other) const
  {
    return x * other.x + y * other.y;
  }

  // Scale and add two vectors
  Vector2D Vector2D::ScaleAdd(const Vector2D& vec0, const Vector2D& vec1, float scale)
  {
    return Vector2D((vec0.x * scale) + vec1.x, (vec0.y * scale) + vec1.y);
  }

  // Scale and subtract two vectors
  Vector2D Vector2D::ScaleSub(const Vector2D& vec0, const Vector2D& vec1, float scale)
  {
    return Vector2D((vec0.x * scale) - vec1.x, (vec0.y * scale) - vec1.y);
  }

  // Scale the vector by a factor
  void Vector2D::Scale(float scale)
  {
    x *= scale;
    y *= scale;
  }

  // Calculate the distance between two vectors
  float Vector2D::Distance(const Vector2D& vec1, const Vector2D& vec2)
  {
    float dx = vec2.x - vec1.x;
    float dy = vec2.y - vec1.y;
    return sqrt(dx * dx + dy * dy);
  }

  // Create a unit vector from an angle in degrees
  Vector2D Vector2D::FromAngleDeg(float angle)
  {
    float radians = angle * ((float)M_PI / 180.0f);
    return Vector2D(cos(radians), sin(radians));
  }

  // Create a unit vector from an angle in radians
  Vector2D Vector2D::FromAngleRad(float angle)
  {
    return Vector2D(cos(angle), sin(angle));
  }

  // Convert a vector to an angle in radians
  float Vector2D::ToAngleRad(const Vector2D& vec)
  {
    return atan2(vec.y, vec.x);
  }

  // Unary negation operator
  Vector2D Vector2D::operator-() const
  {
    return Vector2D(-x, -y);
  }

  // Addition operator
  Vector2D Vector2D::operator+(const Vector2D& other) const
  {
    return Vector2D(x + other.x, y + other.y);
  }

  // Subtraction operator
  Vector2D Vector2D::operator-(const Vector2D& other) const
  {
    return Vector2D(x - other.x, y - other.y);
  }

  // Scalar multiplication operator
  Vector2D Vector2D::operator*(float scale) const
  {
    return Vector2D(x * scale, y * scale);
  }

  // Scalar division operator
  Vector2D Vector2D::operator/(float scale) const
  {
    if (scale != 0.0f)
    {
      float invScale = 1.0f / scale;
      return Vector2D(x * invScale, y * invScale);
    }
    return Vector2D();
  }

  // Assignment operator with initializer list
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
