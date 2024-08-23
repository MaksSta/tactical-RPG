#ifndef CHARACTERS_GOBLINSLINGER_H_
#define CHARACTERS_GOBLINSLINGER_H_

#include "CharacterOnBoard.h"

class GoblinSlinger : public CharacterOnBoard {
public:
  // utworzenie postaci na polu planszy o podanych współrzędnych
  GoblinSlinger(sf::Vector2i start_coords);
};

#endif /* CHARACTERS_GOBLINSLINGER_H_ */
