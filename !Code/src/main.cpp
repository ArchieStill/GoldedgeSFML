#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"


int main()
{
  // std::cout << "You should see a window that opens as well as this writing to console..."
  //           << std::endl;
  // - Removed startup console writing when playing and testing the game as a .exe file

  // create window and set up
  sf::RenderWindow window(sf::VideoMode(1080, 720), "GOLDEDGE");
  window.setFramerateLimit(60);

  //initialise an instance of the game class
  Game game(window);

  //run the playerInitialise function of the game class and check it all initialises ok
  if (!game.init())
  {
    return 0;
  }

  // A Clock starts counting as soon as it's created
  sf::Clock clock;

  // Game loop: run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;

    //calculate delta time
    sf::Time time = clock.restart();
    float dt = time.asSeconds();

    //'process inputs' element of the game loop
    while (window.pollEvent(event))
    {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed)
      {
        game.keyPressed(event);
      }
      if (event.type == sf::Event::KeyReleased)
      {
        game.keyReleased(event);
      }
      if (event.type == sf::Event::MouseButtonReleased)
      {
        game.mouseReleased(event);
      }
    }


    //'animationUpdate' element of the game loop
    game.update(dt);

    window.clear(sf::Color::Black);

    //'render' element of the game loop
    game.render();
    window.display();
  }

  return 0;
}
