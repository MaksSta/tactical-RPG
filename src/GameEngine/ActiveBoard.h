/** 
 * Aktywna część planszy całej planszy, czyli obszar 8x8 obecnie widocznych pól
 */

#ifndef GAME_ENGINE_ACTIVE_BOARD_H_
#define GAME_ENGINE_ACTIVE_BOARD_H_

#include "Field.h"
#include "Level.h"
#include "FullBoard.h"

class ActiveBoard {
public:
    ActiveBoard();

    void loadActiveFields(sf::Vector2i coordsTopLeft, FullBoard& fullBoard);
	
	Field* getFieldByLocalCoords(sf::Vector2i);
    Field* getFieldFromAbsoluteCoords(sf::Vector2i);

	friend class Game;
private:
	Field* field[8][8];
	sf::Vector2i globalCoordsOfTopLeft;
};

#endif /* GAME_ENGINE_ACTIVE_BOARD_H_ */