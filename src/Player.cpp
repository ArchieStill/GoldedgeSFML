
#include "Player.h"

Player::Player()
{
  player_spritesheet.loadFromFile("Data/Images/Adventurer/adventurer-v1.5-Sheet.png");
  sprite->setTexture(player_spritesheet);
  animationsInitialise(&player_spritesheet, sprite, 37, 50);
}

Player::~Player()
{
}

bool Player::playerInitialise()
{
  sprite->setPosition(50, 625);
  sprite->setOrigin(sprite->getPosition().x - 25,sprite->getOrigin().y * 1.15);
  sprite->setScale(2,2);
  attack_range.visibility = false;
  playerAudioInit();
  return true;
}

void Player::update(GameData data)
{
  attackingRange();
  if (attacking)
  {
    attack_range.visibility = true;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !is_crouching)
  {
    sprite->move(-speed * data.dt, 0);
    sprite->setScale(-2,2);
    stateInteger = StateInteger::state2;
    stateChange();
    is_running   = true;
    facing_right = false;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !is_crouching)
  {
    sprite->move(speed * data.dt, 0);
    sprite->setScale(2,2);
    stateInteger = StateInteger::state2;
    stateChange();
    is_running   = true;
    facing_right = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
      sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !is_crouching)
  {
    stateInteger = StateInteger::state1;
    stateChange();
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
  {
    is_crouching = true;
    stateInteger = StateInteger::state4;
    stateChange();
  }

  if (!is_grounded)
  {
    stateInteger = StateInteger::state3;
    stateChange();
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && is_grounded && !is_crouching)
  {
    direction.y = -speed * data.dt;
    is_grounded  = false;
    stateInteger = StateInteger::state3;
    stateChange();
    jump_timer.restart();
    sound.jump_sound.setBuffer(sound.jump_sound_buffer);
    sound.jump_sound.setVolume(100);
    sound.jump_sound.play();
  }

  if (jump_timer.getElapsedTime().asSeconds() > 0.15)
  {
    if (!is_grounded)
    {
      direction.y += speed * data.dt * 0.05;
    }
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    if (sword_equipped && is_grounded && !is_running)
    {
      stateInteger = StateInteger::state5;
      stateChange();
      attacking = true;
    }
  }
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
  {
    is_crouching = true;
    stateInteger = StateInteger::state4;
    stateChange();
  }

  if (sprite->getPosition().y < data.ground_height && is_grounded)
  {
    sprite->move(0, data.gravity_speed / 4);
    is_grounded = false;
  }
  sprite->move(direction.x, direction.y);
  animationUpdate(data.dt);

  if (is_grounded && !is_running)
  {
    stateInteger = StateInteger::state1;
    stateChange();
  }

  if (is_crouching && is_grounded)
  {
    direction.y = -speed * data.dt;
    is_grounded  = false;
    stateInteger = StateInteger::state3;
    stateChange();
    jump_timer.restart();
  }
}

void Player::keyReleased(sf::Event event)
{
  if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
  {
    is_running   = false;
    stateInteger = StateInteger::state1;
    stateChange();
  }
  if (event.key.code == sf::Keyboard::S)
  {
    is_crouching = false;
    stateInteger = StateInteger::state1;
    stateChange();
  }
}
void Player::mouseReleased(sf::Event event)
{
  if (event.mouseButton.button == sf::Mouse::Left)
  {
    if (attacking)
    {
      attacking = false;
      attack_range.visibility = false;
    }
  }
}

void Player::setGrounded()
{
  if (!is_grounded)
  {
    is_grounded  = true;
    direction.y = 0;
    stateInteger = StateInteger::state1;
    stateChange();
  }
}

void Player::playerRender(sf::RenderWindow& window)
{
  window.draw(*sprite);
  if (attack_range.visibility)
  {
    window.draw(*attack_range.getSprite());
  }
}

void Player::attackingRange()
{
  attack_range.initialiseSprite(range_test, "Data/Images/kenney_physicspack/PNG/Explosive elements/elementExplosive011.png");
  attack_range.getSprite()->setColor(sf::Color::Transparent);
  attack_range.getSprite()->setScale(1, 1);
  if (facing_right)
  {
    attack_range.getSprite()->setPosition(sprite->getPosition().x + 25, sprite->getPosition().y - 25);
  }
  else
  {
    attack_range.getSprite()->setPosition(sprite->getPosition().x - 85, sprite->getPosition().y - 25);
  }
}

void Player::playerAudioInit()
{
  if (!sound.jump_sound_buffer.loadFromFile("Data/Sounds/mixkit-quick-jump-arcade-game-239.wav"))
  {
    std::cout << "jump sound did not load \n";
  }
}
