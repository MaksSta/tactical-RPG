#ifndef CHARACTERS_SORCERESS_H_
#define CHARACTERS_SORCERESS_H_

#include "CharacterOnBoard.h"

class Sorceress : public CharacterOnBoard {
public:
    // utworzenie postaci na polu planszy o podanych współrzędnych
    Sorceress(sf::Vector2i start_coords);
};

#endif /* CHARACTERS_SORCERESS_H_ */