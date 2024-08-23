#ifndef CHARACTERS_WARRIOR_H_
#define CHARACTERS_WARRIOR_H_

#include "CharacterOnBoard.h"

class Warrior : public CharacterOnBoard {
public:
  // utworzenie postaci na polu planszy o podanych współrzędnych
  Warrior(sf::Vector2i start_coords);
};

#endif /* CHARACTERS_WARRIOR_H_ */
