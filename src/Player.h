
#ifndef PLATFORMERSFML_PLAYER_H
#define PLATFORMERSFML_PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObject.h"
#include "Animations.h"
#include "Sounds.h"

class Player : public GameObject, public Animations
{
 public:
  Player();
  ~Player();
  bool playerInitialise();
  void keyReleased(sf::Event event);
  void mouseReleased(sf::Event event);

  void update (GameData) override;
  void setGrounded();
  void playerRender(sf::RenderWindow& game_window);
  void attackingRange();
  void playerAudioInit();

  bool is_grounded = true;
  bool is_running = false;
  bool is_crouching= false;
  bool sword_equipped = false;
  bool attacking = false;
  sf::Clock jump_timer;

  GameObject attack_range;
  sf::Texture range_test;
  bool facing_right = true;

 private:
  sf::Texture player_spritesheet;
  Sound sound;
};

#endif // PLATFORMERSFML_PLAYER_H
