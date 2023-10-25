
#include "Animations.h"

Animations::Animations()
{}

Animations::~Animations()
{}

void Animations::animationsInitialise(sf::Texture* _spritesheet, sf::Sprite* _sprite, int _pixels_height, int _pixels_width)
{
  spritesheet = _spritesheet;
  animated_sprite = _sprite;
  pixels_height = _pixels_height;
  pixels_width = _pixels_width;
  playerState = PlayerState::IDLE;

  if (playerState == PlayerState::IDLE)
  {
    current_frame.top = 0;
    current_frame.left = 0;
  }
  else if (playerState == PlayerState::RUNNING)
  {
    current_frame.top = 37;
    current_frame.left = 50;
  }
  else if (playerState == PlayerState::JUMPING)
  {
    current_frame.top = 74;
    current_frame.left = 50;
  }
  else if (playerState == PlayerState::ATTACKING)
  {
    current_frame.top = 259;
    current_frame.left = 0;
  }
  current_frame.height = pixels_height;
  current_frame.width = pixels_width;
  animated_sprite->setTextureRect(current_frame);
}

void Animations::animationUpdate(float dt)
{
  animated_sprite->setTextureRect(current_frame);

  if (playerState == PlayerState::IDLE)
  {
    if (animation_timer.getElapsedTime().asMilliseconds() > 250)
    {
      current_frame.left += 50;
      if (current_frame.left > spritesheet->getSize().x - 200)
      {
        current_frame.left = 0;
      }
      animation_timer.restart();
    }
  }
  else if (playerState == PlayerState::RUNNING)
  {
    if (animation_timer.getElapsedTime().asMilliseconds() > 100)
    {
      current_frame.left += 50;
      if (current_frame.left > spritesheet->getSize().x - 50)
      {
        current_frame.left = 50;
      }
      animation_timer.restart();
    }
  }
  else if (playerState == PlayerState::JUMPING)
  {
    if (animation_timer.getElapsedTime().asMilliseconds() > 75)
    {
      current_frame.left += 50;
      if (current_frame.left > spritesheet->getSize().x - 50)
      {
        current_frame.left = 200;
      }
      animation_timer.restart();
    }
  }
  else if (playerState == PlayerState::ATTACKING)
  {
    if (animation_timer.getElapsedTime().asMilliseconds() > 100)
    {
      current_frame.left += 50;
      if (current_frame.left > spritesheet->getSize().x - 200)
      {
        current_frame.left = 0;
        stateInteger = state1;
        stateChange();
      }
      animation_timer.restart();
    }
  }

  /// ADVENTURER  -   37 x,   50 y,   37 width,   50 height
}

void Animations::stateChange()
{
  switch (stateInteger)
  {
    case state1:
      playerState = PlayerState::IDLE;
      current_frame.top = 0;
      break;
    case state2:
      playerState = PlayerState::RUNNING;
      current_frame.top = 37;
      break;
    case state3:
      playerState = PlayerState::JUMPING;
      current_frame.top = 74;
      break;
    case state4:
      playerState = PlayerState::CROUCHING;
      current_frame.top = 0;
      current_frame.left = 200;
      pixels_height = 10;
      // Crouching does nothing, I was just instinctively pressing S when testing and thought it felt empty without it
      break;
    case state5:
      playerState = PlayerState::ATTACKING;
      current_frame.top = 259;
      break;
    default:
      playerState = PlayerState::IDLE;
      current_frame.top = 0;
      break;
  }
  // std::cout << "state change working: " << static_cast<int>(playerState) << "\n"; (USED FOR TESTING)
}