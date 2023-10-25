
#ifndef PLATFORMERSFML_VECTOR_H
#define PLATFORMERSFML_VECTOR_H

#include <SFML/Graphics.hpp>
#include <cmath>

struct Vector
{
 public:

  // construction
  Vector(float x_, float y_);
  Vector(const Vector& rhs);

  // operations
  Vector operator*(float scalar);
  Vector& operator=(const Vector& rhs);

  // data
  float x = 0;
  float y = 0;

  void enemyVectorL();
  void enemyVectorR();
  void invertX();
};

#endif // PLATFORMERSFML_VECTOR_H
