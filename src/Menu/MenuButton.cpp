#include "MenuButton.h"

MenuButton::MenuButton(std::string str,
                       void (Menu::*method)(),
                       float x,
                       float y,
                       float w,
                       float h)
  :
  onClick{method}
{
  font.loadFromFile("data/fonts/EncodeSansWide-Black.ttf");

  background.setFillColor(sf::Color{0,15,175});
  background.setSize({w, h});

  background.setPosition(x - w / 2,
                         y - h / 2);

  text.setFont(font);
  text.setString(str);
  text.setCharacterSize(80);
  text.setStyle(sf::Text::Bold);
  text.setFillColor(sf::Color{215,215,0});

  text.setOrigin(text.getLocalBounds().getPosition());

  // wierzchołek miejsca gdzie zaczyna się przycisk
  float px = x - w / 2;
  float py = y - h / 2;

  text.setPosition(px + (w - text.getGlobalBounds().width) / 2,
                   py + (h - text.getGlobalBounds().height) / 2);
}

sf::FloatRect MenuButton::getGlobalBounds() const
{
  return background.getGlobalBounds();
}

void MenuButton::MenuButton::draw(sf::RenderTarget &target,
                                  sf::RenderStates states) const
{
  target.draw(background);
  target.draw(text);
}
