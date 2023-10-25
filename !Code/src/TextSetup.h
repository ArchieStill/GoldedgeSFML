#ifndef PLATFORMERSFML_TEXTSETUP_H
#define PLATFORMERSFML_TEXTSETUP_H

#include <SFML/Graphics.hpp>

void TextSetup(sf::Text& text, sf::Font& font, const std::string& string, float x, float y, int char_size, sf::Color color)
{
  text.setString(string);
  text.setFont(font);
  text.setCharacterSize(char_size);
  text.setFillColor(color);
  text.setPosition(x - text.getGlobalBounds().width / 2, y -  text.getGlobalBounds().height);
}

#endif // PLATFORMERSFML_TEXTSETUP_H
