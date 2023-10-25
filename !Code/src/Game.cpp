#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include <iostream>
#include "Game.h"
#include "TextSetup.h"

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}
Game::~Game()
{}

bool Game::init()
{
  gameState = GameState::MAINMENU;
  textInitialise();
  spritesInitialise();
  audioInitialise();
  set_to_default();
  return true;
}

void Game::update(float dt)
{
  data.dt = dt;
  if (gameState == GameState::MAINMENU || gameState == GameState::STORY ||
      gameState == GameState::TUTORIAL || gameState == GameState::CREDITS)
  {
    TextSetup(title,title_font,"GOLDEDGE",window.getSize().x / 2,
              window.getSize().y / 2.25,150,title_colour);
    title_background.getSprite()->move(-1, 0);
    if (title_background.getSprite()->getPosition().x < -1080)
    {
      title_background.getSprite()->setPosition(0, 0);
    }
  }
  if (gameState == GameState::STORY)
  {
    story.move(0, -1);
    if (story.getPosition().y < window.getSize().y / 4)
    {
      story.setPosition(story.getPosition().x, window.getSize().y / 4);
      story_done = true;
    }
  }
  if (gameState == GameState::TUTORIAL)
  {
    player.is_grounded = true;
  }
  if (gameState == GameState::VICTORY3)
  {
    player.is_grounded = true;
    conclusion.move(0, -1);
    if (conclusion.getPosition().y < window.getSize().y / 10)
    {
      conclusion.setPosition(conclusion.getPosition().x,
                             window.getSize().y / 10);
      conclusion_done = true;
    }
  }

  if (gameState == GameState::LEVEL1 || gameState == GameState::LEVEL2 ||
      gameState == GameState::LEVEL3)
  {
    collision();
    blocksCollision();
    coinsCollision();
    enemyCollision();

    /// PLAYER
    player.update(data);
    player.speed = 500;
    if (player.getSprite()->getPosition().x < 50)
    {
      player.getSprite()->setPosition(50, player.getSprite()->getPosition().y);
    }
    if (player.getSprite()->getPosition().x > window.getSize().x - 50)
    {
      player.getSprite()->setPosition(
        window.getSize().x - 50, player.getSprite()->getPosition().y);
    }
    if (
      player.getSprite()->getPosition().y >
      window.getSize().y + player.getSprite()->getGlobalBounds().height ||
      player.getSprite()->getPosition().y < -200)
    {
      lives--;
      lives_display.setString(std::to_string(lives));
      player.getSprite()->setPosition(50, 650);
      isInvincible = true;
      invincible_timer.restart();
      sword.visibility = true;
      player.sword_equipped = false;
      sound.death_sound.setBuffer(sound.death_sound_buffer);
      sound.death_sound.play();
      textInitialise();
    }

    /// ENEMIES
    for (int enemy = 0; enemy < 150; enemy++)
    {
      enemy_array[enemy].update(data);
      if (enemy_array[enemy].getSprite()->getPosition().x < 0)
      {
        enemy_array[enemy].getSprite()->setPosition(
          0, enemy_array[enemy].getSprite()->getPosition().y);
        enemy_array[enemy].getVector()->invertX();
      }
      if (
        enemy_array[enemy].getSprite()->getPosition().x >
        window.getSize().x -
          enemy_array[enemy].getSprite()->getGlobalBounds().width)
      {
        enemy_array[enemy].getSprite()->setPosition(
          window.getSize().x - enemy_array[enemy].getSprite()->getGlobalBounds().width, enemy_array[enemy].getSprite()->getPosition().y);
        enemy_array[enemy].getVector()->invertX();
      }
    }

    if (isInvincible)
    {
      if (invincible_anim.getElapsedTime().asMilliseconds() < 50)
      {
        player.getSprite()->setColor(sf::Color::Transparent);
      }
      if (invincible_anim.getElapsedTime().asMilliseconds() > 150)
      {
        player.getSprite()->setColor(sf::Color::Yellow);
      }
      if (invincible_anim.getElapsedTime().asMilliseconds() > 200)
      {
        invincible_anim.restart();
      }
      if (invincible_timer.getElapsedTime().asSeconds() > 2)
      {
        player.getSprite()->setColor(sf::Color::White);
        isInvincible = false;
      }
    }
  }

  if (gameState == GameState::TUTORIAL || gameState == GameState::LEVEL1)
  {
    sword.getSprite()->setPosition(980, 195);
    sword_light.getSprite()->setPosition(980, 195);
  }
  else if (gameState == GameState::LEVEL2)
  {
    TextSetup(score_display,num_font,std::to_string(score),975,695,35,sf::Color::Black);
    sword.getSprite()->setPosition(50, 50);
    sword_light.getSprite()->setPosition(50, 50);
  }
  else if (gameState == GameState::LEVEL3)
  {
    TextSetup(score_display,num_font,std::to_string(score),975,695,35,sf::Color::Black);
    sword.getSprite()->setPosition(980, 145);
    sword_light.getSprite()->setPosition(980, 145);
  }

  if (enemy_counter == 4 && coin_counter == 20)
  {
    if (gameState == GameState::LEVEL1)
    {
      gameState = GameState::VICTORY1;
      enemy_counter = 0;
      coin_counter = 0;
    }
    else if (gameState == GameState::LEVEL2)
    {
      gameState = GameState::VICTORY2;
      enemy_counter = 0;
      coin_counter = 0;
    }
    else if (gameState == GameState::LEVEL3)
    {
      gameState = GameState::VICTORY3;
    }
  }
  if (lives == 0)
  {
    gameState = GameState::GAMEOVER;
  }

  if (score < 10)
  {
    score_display.setPosition(965, 665);
  }
  else if (score >= 10 && score < 21)
  {
    score_display.setPosition(952.5f, 665);
  }
  sword.getSprite()->setRotation(sword.getSprite()->getRotation() + 2);
  sword_light.getSprite()->setRotation(sword_light.getSprite()->getRotation() - 5);
}

void Game::render()
{
  if (gameState == GameState::MAINMENU)
  {
    title_background.getSprite()->setColor(sf::Color::White);
    window.draw(*title_background.getSprite());
    window.draw(title);
    window.draw(play_option);
    window.draw(credits_option);
    window.draw(quit_option);
  }
  else if (gameState == GameState::STORY)
  {
    title_background.getSprite()->setColor(greyscale);
    window.draw(*title_background.getSprite());
    window.draw(story);
    if (story_done)
    {
      window.draw(story_continue);
    }
  }
  else if (gameState == GameState::TUTORIAL)
  {
    level1_background.getSprite()->setColor(greyscale);
    window.draw(*level1_background.getSprite());
    renderArrays();
    sword.getSprite()->setColor(greyscale);
    window.draw(*sword.getSprite());
    player.playerRender(window);
    player.getSprite()->setPosition(50,615);
    window.draw(*hud.getSprite());
    window.draw(*lives_symbol.getSprite());
    window.draw(lives_display);
    window.draw(*score_symbol.getSprite());
    window.draw(score_display);
    score_display.setPosition(965, 665);
    window.draw(tutorial_move);
    window.draw(tutorial_sword);
    window.draw(tutorial_counters);
    window.draw(story_continue);
  }
  else if (gameState == GameState::CREDITS)
  {
    title_background.getSprite()->setColor(sf::Color::White);
    window.draw(*title_background.getSprite());
    window.draw(credits_title);
    window.draw(credits_text);
    window.draw(credits_exit);
  }
  else if (gameState == GameState::LEVEL1 || gameState == GameState::LEVEL2 ||
           gameState == GameState::LEVEL3)
  {
    if (gameState == GameState::LEVEL1)
    {
      level1_background.getSprite()->setColor(sf::Color::White);
      window.draw(*level1_background.getSprite());
      window.draw(level1_text);
    }
    else if (gameState == GameState::LEVEL2)
    {
      window.draw(*level2_background.getSprite());
      window.draw(level2_text);
    }
    else if (gameState == GameState::LEVEL3)
    {
      level3_background.getSprite()->setColor(sf::Color::White);
      window.draw(*level3_background.getSprite());
      window.draw(level3_text);
    }
    renderArrays();
    if (!player.sword_equipped)
    {
      window.draw(*sword_light.getSprite());
      sword.getSprite()->setColor(sf::Color::White);
      window.draw(*sword.getSprite());
    }
    player.playerRender(window);
    window.draw(*hud.getSprite());
    window.draw(*lives_symbol.getSprite());
    window.draw(lives_display);
    window.draw(*score_symbol.getSprite());
    window.draw(score_display);
  }
  else if (gameState == GameState::GAMEOVER)
  {
    window.draw(defeat_title);
    window.draw(final_score_text);
    window.draw(final_score_calc);
    window.draw(defeat_text);
    window.draw(defeat_quit);
  }
  else if (gameState == GameState::VICTORY1 || gameState == GameState::VICTORY2)
  {
    window.draw(victory_title);
    window.draw(final_score_text);
    window.draw(final_score_calc);
    window.draw(continue_text);
    window.draw(cont_yes);
    window.draw(cont_no);
  }
  else if (gameState == GameState::VICTORY3)
  {
    level3_background.getSprite()->setColor(greyscale);
    window.draw(*level3_background.getSprite());
    renderArrays();
    player.playerRender(window);
    player.getSprite()->setPosition(player.getSprite()->getPosition().x,
                                    player.getSprite()->getPosition().y);
    window.draw(*hud.getSprite());
    window.draw(*lives_symbol.getSprite());
    window.draw(lives_display);
    window.draw(*score_symbol.getSprite());
    window.draw(score_display);
    score_display.setPosition(950, 665);
    window.draw(conclusion);
    if (conclusion_done)
    {
      window.draw(thank_you);
      window.draw(final_score_text);
      final_score_text.setPosition(window.getSize().x / 3.25,
                                   window.getSize().y / 1.1);
      final_score_text.setCharacterSize(45);
      window.draw(final_score_calc);
      final_score_calc.setPosition(window.getSize().x / 2,
                                   window.getSize().y / 1.1);
      final_score_calc.setCharacterSize(40);
    }
  }
}
void Game::renderArrays()
{
  /// Blocks
  for (int blocks = 0; blocks < 150; blocks++)
  {
    if (tiles_array[blocks].visibility)
    {
      window.draw(*tiles_array[blocks].getSprite());
      if (gameState == GameState::LEVEL1)
      {
        tiles_array[blocks].getSprite()->setColor(sf::Color::Cyan);
      }
      else if (gameState == GameState::LEVEL2)
      {
        tiles_array[blocks].getSprite()->setColor(sf::Color::White);
      }
      else if (gameState == GameState::LEVEL3)
      {
        tiles_array[blocks].getSprite()->setColor(sf::Color::Magenta);
      }
      else if (gameState == GameState::TUTORIAL || gameState == GameState::VICTORY3)
      {
        tiles_array[blocks].getSprite()->setColor(greyscale);
      }
    }
  }
  /// Coins
  for (int coins = 0; coins < 150; coins++)
  {
    if (coins_array[coins].visibility)
    {
      window.draw(*coins_array[coins].getSprite());
      if (gameState == GameState::LEVEL1 || gameState == GameState::LEVEL2 ||
          gameState == GameState::LEVEL3)
      {
        coins_array[coins].getSprite()->setColor(sf::Color::White);
      }
      else if (gameState == GameState::TUTORIAL || gameState == GameState::VICTORY3)
      {
        coins_array[coins].getSprite()->setColor(greyscale);
      }
    }
  }
  /// Enemies
  for (int enemy = 0; enemy < 150; enemy++)
  {
    if (enemy_array[enemy].visibility)
    {
      window.draw(*enemy_array[enemy].getSprite());
      if (gameState == GameState::LEVEL1 || gameState == GameState::LEVEL2 ||
          gameState == GameState::LEVEL3)
      {
        enemy_array[enemy].getSprite()->setColor(sf::Color::Red);
      }
      else if (gameState == GameState::TUTORIAL)
      {
        enemy_array[enemy].getSprite()->setColor(greyscale);
      }
    }
  }
}

void Game::keyPressed(sf::Event event)
{
  if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S ||
      event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
  {
    if (gameState == GameState::MAINMENU)
    {
      if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
      {
        menu_integer--;
        titleScreenInitialise();
        if (menu_integer == -1)
        {
          menu_integer = 2;
        }
      }
      else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
      {
        menu_integer++;
        titleScreenInitialise();
        if (menu_integer == 3)
        {
          menu_integer = 0;
        }
      }
    }
  }
  else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
  {
    if (gameState == GameState::VICTORY1 || gameState == GameState::VICTORY2)
    {
      continue_selected = !continue_selected;
      if (continue_selected)
      {
        TextSetup(cont_yes, game_font, "> YES", window.getSize().x / 3,
                  window.getSize().y / 1.25, 50, sf::Color::Green);
        TextSetup(cont_no, game_font, "NO", window.getSize().x / 1.5,
                    window.getSize().y / 1.25, 50, sf::Color::White);
      }
      else
      {
        TextSetup(cont_yes, game_font, "YES", window.getSize().x / 3,
                  window.getSize().y / 1.25, 50, sf::Color::White);
        TextSetup(cont_no, game_font, "> NO", window.getSize().x / 1.5,
                    window.getSize().y / 1.25, 50, sf::Color::Red);
      }
    }
  }
  else if (event.key.code == sf::Keyboard::Enter)
  {
    if (gameState == GameState::MAINMENU)
    {
      if (menu_integer == 0)
      {
        sound.menu_sound.setBuffer(sound.menu_sound_buffer);
        sound.menu_sound.play();
        sound.menu_sound.setVolume(10);
        gameState = GameState::STORY;
        running_timer.restart();
      }
      else if (menu_integer == 1)
      {
        sound.menu_sound.setBuffer(sound.menu_sound_buffer);
        sound.menu_sound.play();
        sound.menu_sound.setVolume(10);
        gameState = GameState::CREDITS;
      }
      else if (menu_integer == 2)
      {
        window.close();
      }
    }
    else if (gameState == GameState::STORY)
    {
      gameState = GameState::TUTORIAL;
      sound.title_music.stop();
      sound.game_music.setBuffer(sound.game_music_buffer);
      sound.game_music.play();
      sound.game_music.setLoop(true);
    }
    else if(gameState == GameState::TUTORIAL)
    {
      gameState = GameState::LEVEL1;
    }
    else if (gameState == GameState::CREDITS)
    {
      sound.menu_sound.setBuffer(sound.menu_sound_buffer);
      sound.menu_sound.play();
      sound.menu_sound.setVolume(10);
      gameState = GameState::MAINMENU;
    }
    else if (gameState == GameState::GAMEOVER)
    {
      gameState = GameState::MAINMENU;
      sound.game_music.stop();
      set_to_default();
    }
    else if (gameState == GameState::VICTORY1)
    {
      if (continue_selected)
      {
        gameState = GameState::LEVEL2;
        level_no++;
        set_to_levelstart();
      }
      else
      {
        gameState = GameState::MAINMENU;
        sound.game_music.stop();
        set_to_default();
      }
    }
    else if (gameState == GameState::VICTORY2)
    {
      if (continue_selected)
      {
        gameState = GameState::LEVEL3;
        level_no++;
        set_to_levelstart();
      }
      else
      {
        gameState = GameState::MAINMENU;
        sound.game_music.stop();
        set_to_default();
      }
    }
    else if (gameState == GameState::VICTORY3)
    {
      if (conclusion_done)
      {
        gameState = GameState::MAINMENU;
        sound.game_music.stop();
        set_to_default();
      }
    }
  }
  else if (event.key.code == sf::Keyboard::Escape)
  {
    window.close();
  }
}
void Game::keyReleased(sf::Event event)
{
  player.keyReleased(event);
}
void Game::mouseReleased(sf::Event event)
{
  player.mouseReleased(event);
}

void Game::collision()
{
  if (player.collision(sword))
  {
    if (sword.visibility)
    {
      sword.visibility = false;
      player.sword_equipped = true;
      sound.get_sword_sound.setBuffer(sound.get_sword_sound_buffer);
      sound.get_sword_sound.play();
    }
  }
}
void Game::blocksCollision()
{
  for (int i = 0; i < 150; i++)
  {
    if (tiles_array[i].visibility)
    {
      if (player.collision(tiles_array[i]))
      {
        if (player.jump_timer.getElapsedTime().asSeconds() > 0.3)
        {
          player.getSprite()->setPosition(
            player.getSprite()->getPosition().x,
            tiles_array[i].getSprite()->getPosition().y -
              player.getSprite()->getGlobalBounds().height / 2);
          player.setGrounded();
        }
      }
    }
  }
}
void Game::coinsCollision()
{
  for (int i = 0; i < 150; i++)
  {
    if (coins_array[i].visibility)
    {
      if (player.collision(coins_array[i]))
      {
        score++;
        score_display.setString(std::to_string(score));
        coins_array[i].visibility = false;
        coin_counter++;
        sound.coin_sound.setBuffer(sound.coin_sound_buffer);
        sound.coin_sound.play();
        textInitialise();
      }
    }
  }
}
void Game::enemyCollision()
{
  for (int i = 0; i < 150; i++)
  {
    if (enemy_array[i].visibility)
    {
      if (player.collision(enemy_array[i]))
      {
        if (!isInvincible)
        {
          player.getSprite()->setPosition(50, 625);
          isInvincible = true;
          invincible_timer.restart();
          lives--;
          lives_display.setString(std::to_string(lives));
          sword.visibility = true;
          player.sword_equipped = false;
          sound.death_sound.setBuffer(sound.death_sound_buffer);
          sound.death_sound.play();
          textInitialise();
        }
      }
      else if (enemy_array[i].collision(player.attack_range))
      {
        if (player.attack_range.visibility)
        {
          enemy_array[i].visibility = false;
          enemy_counter++;
          sound.attack_sound.setBuffer(sound.attack_sound_buffer);
          sound.attack_sound.play();
        }
      }
    }
  }
}

bool Game::textInitialise()
{
  if (!title_font.loadFromFile("Data/Fonts/GethoBold-v0wD.ttf"))
  {
    std::cout << "title font did not load \n";
  }
  if (!game_font.loadFromFile("Data/Fonts/UchronyScBold-mLgLG.ttf"))
  {
    std::cout << "game font did not load \n";
  }
  if (!num_font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout << "score font did not load \n";
  }

  TextSetup(title, title_font, "GOLDEDGE", window.getSize().x / 2,
            window.getSize().y / 2.25, 150, title_colour);
  title.setRotation(355);
  TextSetup(play_option, game_font, "> PLAY", window.getSize().x / 2,
            window.getSize().y / 1.5, 100, sf::Color::Green);
  TextSetup(credits_option, game_font, "CREDITS", window.getSize().x / 2,
            window.getSize().y / 1.25, 75, sf::Color::Black);
  TextSetup(quit_option, game_font, "QUIT", window.getSize().x / 2,
            window.getSize().y / 1.1, 75, sf::Color::Black);

  TextSetup(credits_title, title_font, "CREDITS", window.getSize().x / 2,
            window.getSize().y / 7.5, 110, sf::Color::Blue);
  TextSetup(credits_text, game_font, "Player sprite by rvros on itch.io\nSword sprite by TheWiseHedgehog on itch.io\n"
                                                  "Backgrounds from Kenney Assets\nBlock, coin and enemy sprites from Kenney Assets\n\n"
                                                  "Additional fonts from fontspace.com\nTitle font: Getho (Bold)\nText font: Uchrony SmallCaps (Bold)\n\n"
                                                  "Sound effects from mixkit.co\nMusic by ToastyWithTheMosty on Youtube",
            window.getSize().x / 2,window.getSize().y / 1.25, 35, sf::Color::Black);
  TextSetup(credits_exit, game_font, "Press Enter to return to the main menu.", window.getSize().x / 2,
            window.getSize().y / 1.1, 50, sf::Color::Black);

  TextSetup(defeat_title, title_font, "GAME OVER", window.getSize().x / 2,
            window.getSize().y / 4, 130, sf::Color::Red);
  TextSetup(defeat_text, game_font, "Don't give up! Adventure awaits!", window.getSize().x / 2,
            window.getSize().y / 1.5, 55, sf::Color::Red);
  TextSetup(defeat_quit, game_font, "Press Enter to return to the main menu.", window.getSize().x / 2,
            window.getSize().y / 1.15, 50, sf::Color::White);
  TextSetup(victory_title, title_font, "YOU WIN!", window.getSize().x / 2,
            window.getSize().y / 4, 130, sf::Color::Green);
  TextSetup(final_score_text, game_font, "Final score: ",window.getSize().x / 3,
            window.getSize().y / 2, 60, sf::Color::White);
  TextSetup(final_score_calc, num_font, std::to_string(score) + " x " + std::to_string(lives) + " = " + std::to_string(score * lives),
            window.getSize().x / 1.5,window.getSize().y / 2, 75, sf::Color::Yellow);
  TextSetup(continue_text, game_font, "Would you like to continue?", window.getSize().x / 2,
            window.getSize().y / 1.5, 50, sf::Color::White);
  TextSetup(cont_yes, game_font, "> YES", window.getSize().x / 3,
            window.getSize().y / 1.25, 50, sf::Color::Green);
  TextSetup(cont_no, game_font, "NO", window.getSize().x / 1.5,
            window.getSize().y / 1.25, 50, sf::Color::White);

  TextSetup(score_display, num_font, std::to_string(score), 965, 665, 35, sf::Color::Black);
  TextSetup(lives_display, num_font, std::to_string(lives), 1037.5f, 640, 35, sf::Color::Black);

  TextSetup(story, game_font, "Long ago, the land of Platformus was at peace, kept\n"
                                           "safe from monsters by the light of Goldedge, a mythical\n"
                                           "sword said to one day be passed down to a hero. However,\n"
                                           "the monster army of Badlandia have stolen Goldedge, so\n"
                                           "it's up to you, the chosen hero, to take back the sword\n"
                                           "and save Platformus from destruction once and for all!",
            window.getSize().x / 2, window.getSize().y * 1.5, 50, sf::Color::White);
  TextSetup(story_continue, game_font, "Press Enter to continue.", window.getSize().x / 2,
            window.getSize().y / 1.15, 40, sf::Color::White);
  TextSetup(conclusion, game_font, "And so, with the monsters of Badlandia having been\n"
                                                "defeated at the blade of Goldedge, balance and peace\n"
                                                "was restored to the land of Platformus. Goldedge was\n"
                                                "kept in the possession of the hero, and although no one\n"
                                                "truly knows where the hero vanished to after their\n"
                                                "quest was over, the people of Platformus were forever\n"
                                                "grateful, and knew that they were bound to find more\n"
                                                "adventures with Goldedge by their side...",
            window.getSize().x / 2, window.getSize().y * 1.6, 50, sf::Color::White);
  TextSetup(thank_you, game_font, "THANK YOU FOR PLAYING!", window.getSize().x / 2,
            window.getSize().y / 1.15, 45, sf::Color::Green);

  TextSetup(tutorial_move, game_font, "Press A and D to move. Press Space to jump.", window.getSize().x / 2,
            window.getSize().y / 5, 50, sf::Color::White);
  TextSetup(tutorial_sword, game_font, "Obtain Goldedge to defeat the monsters!\n"
                                                    "             Hold Left Click to attack.",
            window.getSize().x / 2, window.getSize().y / 2.5, 50, sf::Color::White);
  TextSetup(tutorial_counters, game_font, "                 Collect coins to increase your score.\n"
                                                      "Defeat all the enemies and collect all the coins to win.\n"
                                                      "              Lose all your lives and it's GAME OVER!",
            window.getSize().x / 2, window.getSize().y / 1.5, 40, sf::Color::White);

  TextSetup(level1_text, game_font, "Platformus Plains", 150,
            window.getSize().y - 25, 40, sf::Color::Black);
  TextSetup(level2_text, game_font, "Platformus Palace", 150,
            window.getSize().y - 25, 40, sf::Color::Black);
  TextSetup(level3_text, game_font, "Badlandia", 75,
            window.getSize().y - 25, 40, sf::Color::Black);
  return false;
}
bool Game::titleScreenInitialise()
{
  if (menu_integer == 0 || menu_integer == 3)
  {
    TextSetup(play_option, game_font, "> PLAY", window.getSize().x / 2,
              window.getSize().y / 1.5, 100, sf::Color::Green);
    TextSetup(credits_option, game_font, "CREDITS", window.getSize().x / 2,
              window.getSize().y / 1.25, 75, sf::Color::Black);
    TextSetup(quit_option, game_font, "QUIT", window.getSize().x / 2,
              window.getSize().y / 1.1, 75, sf::Color::Black);
  }
  if (menu_integer == 1)
  {
    TextSetup(play_option, game_font, "PLAY", window.getSize().x / 2,
              window.getSize().y / 1.5, 100, sf::Color::Black);
    TextSetup(credits_option, game_font, "> CREDITS", window.getSize().x / 2,
              window.getSize().y / 1.25, 75, sf::Color::Blue);
    TextSetup(quit_option, game_font, "QUIT", window.getSize().x / 2,
              window.getSize().y / 1.1, 75, sf::Color::Black);
  }
  if (menu_integer == 2 || menu_integer == -1)
  {
    TextSetup(play_option, game_font, "PLAY", window.getSize().x / 2,
              window.getSize().y / 1.5, 100, sf::Color::Black);
    TextSetup(credits_option, game_font, "CREDITS", window.getSize().x / 2,
              window.getSize().y / 1.25, 75, sf::Color::Black);
    TextSetup(quit_option, game_font, "> QUIT", window.getSize().x / 2,
              window.getSize().y / 1.1, 75, sf::Color::Red);
  }
  return false;
}
bool Game::spritesInitialise()
{
  player.playerInitialise();
  player.getSprite()->setOrigin(player.getSprite()->getLocalBounds().width / 2,player.getSprite()->getLocalBounds().height / 2);

  sword.initialiseSprite(sword_texture, "Data/Images/Sword.png");
  sword.getSprite()->setPosition(980, 195);
  sword.getSprite()->setScale(0.15f, 0.15f);
  sword.getSprite()->setOrigin(sword.getSprite()->getLocalBounds().width / 2, sword.getSprite()->getLocalBounds().height / 2);
  sword_light.initialiseSprite(light_texture, "Data/Images/kenney_physicspack/PNG/Other/starGold.png");
  sword_light.getSprite()->setPosition(980, 195);
  sword_light.getSprite()->setScale(1.5f, 1.5f);
  sword_light.getSprite()->setOrigin(sword_light.getSprite()->getLocalBounds().width / 2, sword_light.getSprite()->getLocalBounds().height / 2);

  hud.initialiseSprite(hud_texture, "Data/Images/kenney_physicspack/PNG/Metal elements/elementMetal012.png");
  hud.getSprite()->setPosition(window.getSize().x - 260, window.getSize().y + 85);
  hud.getSprite()->setRotation(320);
  hud.getSprite()->setScale(3, 2);
  hud.getSprite()->setColor(sf::Color::Blue);
  lives_symbol.initialiseSprite(lives_texture, "Data/Images/Life Symbol.png");
  lives_symbol.getSprite()->setPosition(window.getSize().x - 80, window.getSize().y - 125);
  lives_symbol.getSprite()->setScale(1, 1);
  score_symbol.initialiseSprite(score_texture, "Data/Images/kenney_physicspack/PNG/Other/coinGold.png");
  score_symbol.getSprite()->setPosition(window.getSize().x - 160, window.getSize().y - 90);
  score_symbol.getSprite()->setScale(1.5f,1.5f);

  title_background.initialiseSprite(title_background_texture, "Data/Images/kenney_physicspack/PNG/Backgrounds/colored_grass.png");
  title_background.getSprite()->setPosition(0,0);
  title_background.getSprite()->setScale(1.055, 1);
  title_background_texture.setRepeated(true);
  title_background.getSprite()->setTextureRect(sf::IntRect(0,0,2048,720));
  level1_background.initialiseSprite(
    level1_background_texture, "Data/Images/lvl1.png");
  level1_background.getSprite()->setPosition(0,-125);
  level1_background.getSprite()->setScale(1, 1);
  level2_background.initialiseSprite(
    level2_background_texture, "Data/Images/lvl2.png");
  level2_background.getSprite()->setPosition(-400,-125);
  level2_background.getSprite()->setScale(1, 1);
  level3_background.initialiseSprite(
    level3_background_texture, "Data/Images/lvl3.png");
  level3_background.getSprite()->setPosition(-400,-125);
  level3_background.getSprite()->setScale(1, 1);
  return false;
}
bool Game::audioInitialise()
{
  if (!sound.coin_sound_buffer.loadFromFile("Data/Sounds/mixkit-game-treasure-coin-2038.wav"))
  {
    std::cout << "coin sound did not load \n";
  }
  if (!sound.get_sword_sound_buffer.loadFromFile("Data/Sounds/mixkit-arcade-score-interface-217.wav"))
  {
    std::cout << "get sword sound did not load \n";
  }
  if (!sound.attack_sound_buffer.loadFromFile("Data/Sounds/mixkit-swift-sword-strike-2166.wav"))
  {
    std::cout << "attack sound did not load \n";
  }
  if (!sound.death_sound_buffer.loadFromFile("Data/Sounds/mixkit-wrong-answer-fail-notification-946.wav"))
  {
    std::cout << "death sound did not load \n";
  }
  if (!sound.menu_sound_buffer.loadFromFile("Data/Sounds/mixkit-arcade-bonus-alert-767.wav"))
  {
    std::cout << "menu sound did not load \n";
  }

  if (!sound.title_music_buffer.loadFromFile("Data/Sounds/Hometown - Xenoblade Chronicles [NES] 8-bit.ogg"))
  {
    std::cout << "title music did not load \n";
  }
  if (!sound.game_music_buffer.loadFromFile("Data/Sounds/You Will Know Our Names - Xenoblade Chronicles [NES] 8-bit.ogg"))
  {
    std::cout << "game music did not load \n";
  }
  return true;
}

bool Game::blocksInitialise()
{
  tiles_array = new GameObject[150];
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 15; j++)
    {
      tiles_array[j + i * 15].visibility = false;
      tiles_array[j + i * 15].initialiseSprite(tiles_texture, "Data/Images/kenney_physicspack/PNG/Stone elements/elementStone018.png");
      tiles_array[j + i * 15].getSprite()->setPosition(15 + j * 70, 15 + 70 * i);
      tiles_array[j + i * 15].getSprite()->setScale(0.5f, 0.1f);
    }
  }
  if (level_no == 0)
  {
    for (int L1start = 135; L1start < 140; L1start++)
    {
      tiles_array[L1start].visibility = true;
    }
    for (int L1plat1 = 142; L1plat1 < 145; L1plat1++)
    {
      tiles_array[L1plat1].visibility = true;
    }
    for (int L1plat2 = 116; L1plat2 < 120; L1plat2++)
    {
      tiles_array[L1plat2].visibility = true;
    }
    for (int L1plat3 = 80; L1plat3 < 84; L1plat3++)
    {
      tiles_array[L1plat3].visibility = true;
    }
    for (int L1plat4 = 105; L1plat4 < 108; L1plat4++)
    {
      tiles_array[L1plat4].visibility = true;
    }
    for (int L1plat5 = 47; L1plat5 < 51; L1plat5++)
    {
      tiles_array[L1plat5].visibility = true;
    }
    for (int L1plat6 = 15; L1plat6 < 17; L1plat6++)
    {
      tiles_array[L1plat6].visibility = true;
    }
    for (int L1plat7 = 23; L1plat7 < 27; L1plat7++)
    {
      tiles_array[L1plat7].visibility = true;
    }
    for (int L1end = 58; L1end < 60; L1end++)
    {
      tiles_array[L1end].visibility = true;
    }
  }
  else if (level_no == 1)
  {
    for (int L2start = 135; L2start < 138; L2start++)
    {
      tiles_array[L2start].visibility = true;
    }
    for (int L2plat1 = 140; L2plat1 < 142; L2plat1++)
    {
      tiles_array[L2plat1].visibility = true;
    }
    for (int L2plat2 = 144; L2plat2 < 146; L2plat2++)
    {
      tiles_array[L2plat2].visibility = true;
    }
    for (int L2plat3 = 110; L2plat3 < 112; L2plat3++)
    {
      tiles_array[L2plat3].visibility = true;
    }
    for (int L2plat4 = 116; L2plat4 < 119; L2plat4++)
    {
      tiles_array[L2plat4].visibility = true;
    }
    for (int L2plat5 = 75; L2plat5 < 78; L2plat5++)
    {
      tiles_array[L2plat5].visibility = true;
    }
    for (int L2plat6 = 84; L2plat6 < 86; L2plat6++)
    {
      tiles_array[L2plat6].visibility = true;
    }
    for (int L2plat7 = 46; L2plat7 < 49; L2plat7++)
    {
      tiles_array[L2plat7].visibility = true;
    }
    for (int L2plat8 = 51; L2plat8 < 54; L2plat8++)
    {
      tiles_array[L2plat8].visibility = true;
    }
    for (int L2plat9 = 56; L2plat9 < 59; L2plat9++)
    {
      tiles_array[L2plat9].visibility = true;
    }
    for (int L2plat10 = 24; L2plat10 < 26; L2plat10++)
    {
      tiles_array[L2plat10].visibility = true;
    }
    for (int L2end = 15; L2end < 19; L2end++)
    {
      tiles_array[L2end].visibility = true;
    }
  }
  else if (level_no == 2)
  {
    for (int L3start = 135; L3start < 138; L3start++)
    {
      tiles_array[L3start].visibility = true;
    }
    for (int L3plat1 = 140; L3plat1 < 143; L3plat1++)
    {
      tiles_array[L3plat1].visibility = true;
    }
    for (int L3plat2 = 147; L3plat2 < 150; L3plat2++)
    {
      tiles_array[L3plat2].visibility = true;
    }
    for (int L3plat3 = 106; L3plat3 < 109; L3plat3++)
    {
      tiles_array[L3plat3].visibility = true;
    }
    for (int L3plat4 = 114; L3plat4 < 116; L3plat4++)
    {
      tiles_array[L3plat4].visibility = true;
    }
    for (int L3plat5 = 75; L3plat5 < 78; L3plat5++)
    {
      tiles_array[L3plat5].visibility = true;
    }
    for (int L3plat6 = 80; L3plat6 < 83; L3plat6++)
    {
      tiles_array[L3plat6].visibility = true;
    }
    for (int L3plat7 = 46; L3plat7 < 49; L3plat7++)
    {
      tiles_array[L3plat7].visibility = true;
    }
    for (int L3plat8 = 54; L3plat8 < 56; L3plat8++)
    {
      tiles_array[L3plat8].visibility = true;
    }
    for (int L3plat9 = 15; L3plat9 < 18; L3plat9++)
    {
      tiles_array[L3plat9].visibility = true;
    }
    for (int L3plat10 = 20; L3plat10 < 23; L3plat10++)
    {
      tiles_array[L3plat10].visibility = true;
    }
    for (int L3plat11 = 27; L3plat11 < 30; L3plat11++)
    {
      tiles_array[L3plat11].visibility = true;
    }
    for (int L3end = 88; L3end < 90; L3end++)
    {
      tiles_array[L3end].visibility = true;
    }
  }
  return false;
}
bool Game::coinsInitialise()
{
  coins_array = new GameObject[150];
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 15; j++)
    {
      coins_array[j + i * 15].visibility = false;
      coins_array[j + i * 15].initialiseSprite(coins_texture, "Data/Images/kenney_physicspack/PNG/Other/coinGold.png");
      coins_array[j + i * 15].getSprite()->setPosition(10 + j * 70, 15 + 70 * i);
      coins_array[j + i * 15].getSprite()->setScale(1, 1);
    }
  }
  if (level_no == 0)
  {
    for (int L1coin1 = 121; L1coin1 < 125; L1coin1++)
    {
      coins_array[L1coin1].visibility = true;
    }
    for (int L1coin2 = 127; L1coin2 < 130; L1coin2++)
    {
      coins_array[L1coin2].visibility = true;
    }
    for (int L1coin3 = 102; L1coin3 < 104; L1coin3++)
    {
      coins_array[L1coin3].visibility = true;
    }
    for (int L1coin4 = 66; L1coin4 < 68; L1coin4++)
    {
      coins_array[L1coin4].visibility = true;
    }
    for (int L1coin5 = 90; L1coin5 < 93; L1coin5++)
    {
      coins_array[L1coin5].visibility = true;
    }
    for (int L1coin6 = 33; L1coin6 < 35; L1coin6++)
    {
      coins_array[L1coin6].visibility = true;
    }
    for (int L1coin7 = 0; L1coin7 < 2; L1coin7++)
    {
      coins_array[L1coin7].visibility = true;
    }
    for (int L1coin8 = 9; L1coin8 < 11; L1coin8++)
    {
      coins_array[L1coin8].visibility = true;
    }
  }
  else if (level_no == 1)
  {
    for (int L2coin1 = 121; L2coin1 < 123; L2coin1++)
    {
      coins_array[L2coin1].visibility = true;
    }
    for (int L2coin2 = 125; L2coin2 < 127; L2coin2++)
    {
      coins_array[L2coin2].visibility = true;
    }
    for (int L2coin3 = 129; L2coin3 < 131; L2coin3++)
    {
      coins_array[L2coin3].visibility = true;
    }
    for (int L2coin4 = 95; L2coin4 < 97; L2coin4++)
    {
      coins_array[L2coin4].visibility = true;
    }
    for (int L2coin5 = 102; L2coin5 < 103; L2coin5++)
    {
      coins_array[L2coin5].visibility = true;
    }
    for (int L2coin6 = 61; L2coin6 < 62; L2coin6++)
    {
      coins_array[L2coin6].visibility = true;
    }
    for (int L2coin7 = 69; L2coin7 < 71; L2coin7++)
    {
      coins_array[L2coin7].visibility = true;
    }
    for (int L2coin8 = 32; L2coin8 < 33; L2coin8++)
    {
      coins_array[L2coin8].visibility = true;
    }
    for (int L2coin9 = 37; L2coin9 < 38; L2coin9++)
    {
      coins_array[L2coin9].visibility = true;
    }
    for (int L2coin10 = 42; L2coin10 < 43; L2coin10++)
    {
      coins_array[L2coin10].visibility = true;
    }
    for (int L2coin11 = 1; L2coin11 < 4; L2coin11++)
    {
      coins_array[L2coin11].visibility = true;
    }
    for (int L2coin12 = 9; L2coin12 < 11; L2coin12++)
    {
      coins_array[L2coin12].visibility = true;
    }
  }
  else if (level_no == 2)
  {
    for (int L3coin1 = 121; L3coin1 < 122; L3coin1++)
    {
      coins_array[L3coin1].visibility = true;
    }
    for (int L3coin2 = 126; L3coin2 < 127; L3coin2++)
    {
      coins_array[L3coin2].visibility = true;
    }
    for (int L3coin3 = 132; L3coin3 < 135; L3coin3++)
    {
      coins_array[L3coin3].visibility = true;
    }
    for (int L3coin4 = 92; L3coin4 < 93; L3coin4++)
    {
      coins_array[L3coin4].visibility = true;
    }
    for (int L3coin5 = 99; L3coin5 < 101; L3coin5++)
    {
      coins_array[L3coin5].visibility = true;
    }
    for (int L3coin6 = 61; L3coin6 < 62; L3coin6++)
    {
      coins_array[L3coin6].visibility = true;
    }
    for (int L3coin7 = 66; L3coin7 < 67; L3coin7++)
    {
      coins_array[L3coin7].visibility = true;
    }
    for (int L3coin8 = 32; L3coin8 < 33; L3coin8++)
    {
      coins_array[L3coin8].visibility = true;
    }
    for (int L3coin9 = 39; L3coin9 < 41; L3coin9++)
    {
      coins_array[L3coin9].visibility = true;
    }
    for (int L3coin10 = 1; L3coin10 < 2; L3coin10++)
    {
      coins_array[L3coin10].visibility = true;
    }
    for (int L3coin11 = 6; L3coin11 < 7; L3coin11++)
    {
      coins_array[L3coin11].visibility = true;
    }
    for (int L3coin12 = 12; L3coin12 < 15; L3coin12++)
    {
      coins_array[L3coin12].visibility = true;
    }
    for (int L3coin13 = 73; L3coin13 < 75; L3coin13++)
    {
      coins_array[L3coin13].visibility = true;
    }
  }
  return false;
}
bool Game::enemyInitialise()
{
  enemy_array = new GameObject[150];
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 15; j++)
    {
      enemy_array[j + i * 15].visibility = false;
      enemy_array[j + i * 15].initialiseSprite(enemy_texture, "Data/Images/kenney_physicspack/PNG/Aliens/alienBlue_square.png");
      enemy_array[j + i * 15].getSprite()->setPosition(10 + j * 70, 15 + 70 * i);
      enemy_array[j + i * 15].getSprite()->setScale(0.75f, 0.75f);
      enemy_array[j + i * 15].getSprite()->setColor(sf::Color::Red);
    }
  }
  for (int enemy1 = 91; enemy1 < 92; enemy1++)
  {
    enemy_array[enemy1].visibility = true;
    enemy_array[enemy1].getVector()->enemyVectorL();
    enemy_array[enemy1].speed = 400;
  }
  for (int enemy2 = 71; enemy2 < 72; enemy2++)
  {
    enemy_array[enemy2].visibility = true;
    enemy_array[enemy2].getVector()->enemyVectorR();
    enemy_array[enemy2].speed = 500;
  }
  for (int enemy3 = 36; enemy3 < 37; enemy3++)
  {
    enemy_array[enemy3].visibility = true;
    enemy_array[enemy3].getVector()->enemyVectorL();
    enemy_array[enemy3].speed = 600;
  }
  for (int enemy4 = 3; enemy4 < 4; enemy4++)
  {
    enemy_array[enemy4].visibility = true;
    enemy_array[enemy4].getVector()->enemyVectorR();
    enemy_array[enemy4].speed = 700;
  }
  return false;
}

bool Game::set_to_levelstart()
{
  textInitialise();
  blocksInitialise();
  coinsInitialise();
  enemyInitialise();
  continue_selected = true;
  isInvincible = false;
  player.getSprite()->setColor(sf::Color::White);
  sword.visibility = true;
  player.sword_equipped = false;
  player.getSprite()->setPosition(50, 630);
  player.getSprite()->setScale(2,2);
  return false;
}
bool Game::set_to_default()
{
  level_no = 0;
  blocksInitialise();
  coinsInitialise();
  enemyInitialise();
  sound.title_music.setBuffer(sound.title_music_buffer);
  sound.title_music.play();
  sound.title_music.setLoop(true);
  sound.title_music.setVolume(500);
  menu_integer = 0;
  continue_selected = true;
  isInvincible = false;
  player.getSprite()->setColor(sf::Color::White);
  sword.visibility = true;
  player.sword_equipped = false;
  enemy_counter = 0;
  coin_counter = 0;
  score = 0;
  score_display.setString(std::to_string(score));
  lives = 5;
  lives_display.setString(std::to_string(lives));
  story_done = false;
  conclusion_done = false;
  return false;
}

#pragma clang diagnostic pop