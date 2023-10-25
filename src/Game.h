
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"

enum class GameState
{
  MAINMENU,
  STORY,
  TUTORIAL,
  LEVEL1,
  LEVEL2,
  LEVEL3,
  VICTORY1,
  VICTORY2,
  VICTORY3,
  GAMEOVER,
  CREDITS
};

class Game
{
 public:
  GameState gameState;
  GameData data;
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void renderArrays();
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);
  void mouseReleased(sf::Event event);

  void collision();
  void blocksCollision();
  void coinsCollision();
  void enemyCollision();

  bool textInitialise();
  bool titleScreenInitialise();
  bool spritesInitialise();
  bool audioInitialise();

  bool blocksInitialise();
  bool coinsInitialise();
  bool enemyInitialise();

  bool set_to_levelstart();
  bool set_to_default();

 private:
  sf::RenderWindow& window;

  sf::Font title_font;
  sf::Font game_font;
  sf::Font num_font;

  sf::Text title;
  sf::Color title_colour = sf::Color(207, 167, 0, 255);
  sf::Text play_option;
  sf::Text credits_option;
  sf::Text quit_option;
  int menu_integer = 0;

  sf::Text credits_title;
  sf::Text credits_text;
  sf::Text credits_exit;

  sf::Text story;
  sf::Color greyscale = sf::Color(50, 50, 50, 255);
  sf::Text story_continue;
  bool story_done = false;
  sf::Text conclusion;
  sf::Text thank_you;
  bool conclusion_done = false;

  sf::Text tutorial_move;
  sf::Text tutorial_sword;
  sf::Text tutorial_counters;

  GameObject title_background;
  sf::Texture title_background_texture;
  GameObject level1_background;
  sf::Texture level1_background_texture;
  sf::Text level1_text;
  GameObject level2_background;
  sf::Texture level2_background_texture;
  sf::Text level2_text;
  GameObject level3_background;
  sf::Texture level3_background_texture;
  sf::Text level3_text;
  int level_no;

  Player player;
  sf::Clock running_timer;
  GameObject sword;
  sf::Texture sword_texture;
  GameObject sword_light;
  sf::Texture light_texture;

  sf::Clock invincible_timer;
  sf::Clock invincible_anim;
  bool isInvincible = false;

  GameObject *tiles_array;
  sf::Texture tiles_texture;
  GameObject *coins_array;
  sf::Texture coins_texture;
  GameObject *enemy_array;
  sf::Texture enemy_texture;
  int enemy_counter = 0;
  int coin_counter = 0;

  sf::Text defeat_title;
  sf::Text defeat_text;
  sf::Text defeat_quit;
  sf::Text victory_title;
  sf::Text final_score_text;
  sf::Text final_score_calc;
  sf::Text continue_text;
  sf::Text cont_yes;
  sf::Text cont_no;
  bool continue_selected;

  GameObject hud;
  sf::Texture hud_texture;
  int score = 0;
  sf::Text score_display;
  GameObject score_symbol;
  sf::Texture score_texture;
  int lives = 5;
  sf::Text lives_display;
  GameObject lives_symbol;
  sf::Texture lives_texture;

  Sound sound;
};

#endif // PLATFORMER_GAME_H
