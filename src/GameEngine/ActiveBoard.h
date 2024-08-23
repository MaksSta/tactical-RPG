/**
 * Obszar 8x8 obecnie widocznych pól na których można wykonywać w grze akcje.
 *
 * Przed rozpoczęciem użyciem klasy, należy załadować ją metodą loadActiveFields
 * i wywoływać ją za każdym razem gdy chce się pobrać inną część pełnej planszy.
 */

#ifndef GAME_ENGINE_ACTIVEBOARD_H_
#define GAME_ENGINE_ACTIVEBOARD_H_

#include "Field.h"
#include "FullBoard.h"

class ActiveBoard {
public:
  /**
   * @param coordsTopLeft współrzędne, które odzwierciedlają pole {0, 0} na pełnej planszy gry
   * @param fullBoard pełna plansza gry z której pobierane są wskaźniki do pól
   */
  void loadActiveFields(sf::Vector2i coordsTopLeft,
                        FullBoard& fullBoard);

  Field* getField(sf::Vector2i coords) const;
  sf::Vector2i getCoordsOf(Field* checked_field) const;

  friend class Game;
private:
  Field* field[8][8];
};

namespace errors
{
  struct field_not_found{};
}

#endif /* GAME_ENGINE_ACTIVEBOARD_H_ */
