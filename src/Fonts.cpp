#include "Fonts.h"

sf::Font font_normal;
sf::Font font_rpg;

void loadFonts()
{
  font_normal.loadFromFile("data/fonts/EncodeSansWide-Black.ttf");
  font_rpg.loadFromFile("data/fonts/BreatheFireIii-PKLOB.ttf");
}
