
#include "Vector.h"
#include <math.h>

Vector::Vector(float x_, float y_) : x(x_), y(y_) {}

Vector::Vector(const Vector& rhs)
{
  this->x = rhs.x;
  this->y = rhs.y;
}

Vector& Vector::operator=(const Vector& rhs)
{
  this->x = rhs.x;
  this->y = rhs.y;

  return *this;
}

Vector Vector::operator*(float scalar)
{
  Vector vec(*this);
  vec.x *= scalar;
  vec.y *= scalar;
  return vec;
}

void Vector::enemyVectorL()
{
  x = -1;
  y = 0;
}
void Vector::enemyVectorR()
{
  x = 1;
  y = 0;
}
void Vector::invertX()
{
  x = -x;
}
