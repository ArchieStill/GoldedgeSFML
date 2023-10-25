
#ifndef PLATFORMERSFML_GAMEOBJECT_H
#define PLATFORMERSFML_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "Vector.h"
#include "GameData.h"

class GameObject
{
 public:
  GameObject();
  ~GameObject();
  virtual void update(GameData);
  bool initialiseSprite(sf::Texture &texture, std::string filename);
  sf::Sprite* getSprite();
  Vector* getVector();
  bool visibility;
  int speed;
  Vector direction {0,0};

  bool collision(GameObject& collided);

 protected:
  sf::Sprite* sprite = nullptr;
};

#endif // PLATFORMERSFML_GAMEOBJECT_H
