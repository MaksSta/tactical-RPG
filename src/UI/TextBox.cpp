#include "TextBox.h"

TextBox::TextBox(sf::Font& font_inside,
                 sf::Font& font_outside,
                 sf::Vector2f pos,
                 sf::Vector2f size,
                 Alignment _alignment_horizontal,
                 std::string text_label,
                 std::string text,
                 sf::Color color,
                 unsigned int font_size)
  :
  alignment_horizontal{_alignment_horizontal},
  label{font_inside, pos - sf::Vector2f{10, 25}, text_label, 16},
  textfield{font_outside, pos, text}
{
  textfield.setFillColor(color);
  textfield.setCharacterSize(font_size);

  background.setPosition(pos);
  background.setSize(size);
  background.setFillColor(sf::Color{95, 95, 95});

  refresh();
}

void TextBox::setString(std::string text)
{
  textfield.setString(text);
  refresh();
}

void TextBox::refresh()
{
  auto pos = background.getPosition();
  auto size = background.getSize();

  float posX;
  switch (alignment_horizontal) {
  case Alignment::start: {
    posX = pos.x + margin;
  } break;
  case Alignment::center: {
    posX = pos.x + size.x / 2 - textfield.getGlobalBounds().width / 2;
  } break;
  case Alignment::end: {
    posX = pos.x + size.x - textfield.getGlobalBounds().width - margin;
  } break;
  }

  // wyśrodkuj oś x według podanego alignmentu, oś y ma alignment center
  textfield.setPosition(posX,
                        pos.y + size.y / 2 - textfield.getGlobalBounds().height / 2);
}

void TextBox::draw(sf::RenderTarget &target,
                   sf::RenderStates states) const
{
  target.draw(background);
  target.draw(label);
  target.draw(textfield);
}
