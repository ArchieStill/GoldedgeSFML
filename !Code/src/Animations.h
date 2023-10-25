
#ifndef PLATFORMERSFML_ANIMATIONS_H
#define PLATFORMERSFML_ANIMATIONS_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum class PlayerState
{
  IDLE,
  RUNNING,
  JUMPING,
  CROUCHING,
  ATTACKING
};

enum StateInteger
{
  state1,
  state2,
  state3,
  state4,
  state5
};

class Animations
{
 public:
  PlayerState playerState;
  StateInteger stateInteger;
  Animations();
  ~Animations();

  void animationsInitialise(sf::Texture*, sf::Sprite*, int, int);
  void animationUpdate(float dt);
  void stateChange();

 private:
  sf::Texture* spritesheet;
  sf::Sprite* animated_sprite;
  int pixels_height;
  int pixels_width;
  sf::IntRect current_frame;
  sf::Clock animation_timer;
};

#endif // PLATFORMERSFML_ANIMATIONS_H
