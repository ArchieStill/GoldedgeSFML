
#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
  visibility = true;
  sprite = new sf::Sprite;
}

GameObject::~GameObject()
{
  delete sprite;
  sprite = nullptr;
}

bool GameObject::initialiseSprite(sf::Texture& texture, std::string filename)
{
  speed = 100;
  if (!texture.loadFromFile(filename))
  {
    std::cout << "texture did not load \n";
    return false;
  }
  sprite->setTexture(texture);
  return true;
}

sf::Sprite* GameObject::getSprite()
{
  return sprite;
}
Vector* GameObject::getVector()
{
  return &direction;
}

void GameObject::update(GameData data)
{
  if (sprite != nullptr)
  {
    sprite->move(direction.x * speed * data.dt, direction.y * speed * data.dt);
    if (visibility)
    {
      sprite->getTexture();
    }
  }
}
bool GameObject::collision(GameObject& collided)
{
  if (sprite->getPosition().x + sprite->getGlobalBounds().width / 4 < collided.sprite->getPosition().x ||
      sprite->getPosition().x > collided.sprite->getPosition().x + collided.sprite->getGlobalBounds().width)
  {
    return false;
  }
  if (sprite->getPosition().y + sprite->getGlobalBounds().height / 2 < collided.sprite->getPosition().y ||
      sprite->getPosition().y > collided.sprite->getPosition().y + collided.sprite->getGlobalBounds().height)
  {
    return false;
  }
  return true;
}