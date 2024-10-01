#include "Ability.h"

using namespace Abilities;

std::vector<sf::Vector2i> Ability::get_in_range() const
{
  return in_range;
}
