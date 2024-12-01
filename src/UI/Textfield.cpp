#include "Textfield.h"

Textfield::Textfield(sf::Font& font,
                     sf::Vector2f pos,
                     std::string text,
                     unsigned int font_size,
                     sf::Color color)
{
  setPosition(pos);

  setFont(font);
  setCharacterSize(font_size);
  setString(text);
  setFillColor(color);
}
