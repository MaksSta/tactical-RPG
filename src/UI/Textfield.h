/**
 * Tekst do wyświetlenia na ekranie z załadowaną, domyślną czcionką
 */

#ifndef UI_TEXTFIELD_H_
#define UI_TEXTFIELD_H_

#include <SFML/Graphics.hpp>
#include "../Fonts.h"

class Textfield : public sf::Text {
public:
  /**
   * Tworzy obiekt samego tekstu na ekranie
   * \param pos pozycja na ekranie
   * \param text tekst do wyświetlenia
   */
  Textfield(sf::Font& font = fonts->font_normal,
            sf::Vector2f pos = {0, 0},
            std::string text = "",
            unsigned int font_size = 24,
            sf::Color color = sf::Color::White);
};

#endif /* UI_TEXTFIELD_H_ */
