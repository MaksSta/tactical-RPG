#ifndef FONTS_H_
#define FONTS_H_

#include <SFML/Graphics.hpp>

struct Fonts {
  void loadFonts();

  sf::Font font_normal;
  sf::Font font_rpg;
};

extern Fonts* fonts;

#endif /* FONTS_H_ */
