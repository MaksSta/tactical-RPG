/**
 * Bazowa klasa dla wszystkich umiejętności jaką może wywoływać postać
 */

#ifndef ABILITIES_ABILITY_H_
#define ABILITIES_ABILITY_H_

#include <SFML/Graphics.hpp>

#include "../Characters/Character.h"
#include "../Global.h"

class Ability {
public:
  // zwraca zasięg wywołania umejetności podany w wektorze przesunięć względem pola postaci
  std::vector<sf::Vector2i> get_in_range();
protected:
  // zasięg wywołania umiejetności podany w wektorze przesunięć względem pola postaci
  std:: vector<sf::Vector2i> in_range;
};

#endif /* ABILITIES_ABILITY_H_ */
