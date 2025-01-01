#include "Fonts.h"

Fonts* fonts;

void Fonts::loadFonts()
{
  font_normal.loadFromFile("data/fonts/EncodeSansWide-Black.ttf");
  font_rpg.loadFromFile("data/fonts/BreatheFireIii-PKLOB.ttf");
}
