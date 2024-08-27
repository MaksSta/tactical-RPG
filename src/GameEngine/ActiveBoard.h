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
#include "../Characters/CharacterOnBoard.h"
#include "Range.h"

class ActiveBoard {
public:
  /**
   * @param coordsTopLeft współrzędne, które odzwierciedlają pole {0, 0} na pełnej planszy gry
   * @param fullBoard pełna plansza gry z której pobierane są wskaźniki do pól
   */
  void loadActiveFields(sf::Vector2i coordsTopLeft,
                        FullBoard& fullBoard);

  void addCharacter(CharacterOnBoard* characters);

  Field* getField(sf::Vector2i coords) const;
  sf::Vector2i getCoordsOf(Field* checked_field) const;

  Field* getFieldOccupedBy(CharacterOnBoard*) const;

  // sprawdzenie czy na podanym polu znajduje się postać, jeśli tak zwraca wskaźnik do niej
  CharacterOnBoard* getCharacterOnField(Field* field) const;

  // sprawdzenie czy podane pole znajduje się w zasięgu obecnie wybranego do wykonania ataku
  bool isFieldInRange(Field* field,
                      Range& range);

  // utworzenie obiektu range na podstawie podanych przesunięć (względem selectedCharacter)
  Range createRange(CharacterOnBoard* character,
                    std::vector<sf::Vector2i> in_range);


  // pobranie zajętych pól na planszy
  std::vector<Field*> getBlockedFields() const;

  // zwraca tylko żyjące postacie
  std::vector<CharacterOnBoard*> getAliveCharacters() const;

  friend class Game;
private:
  Field* field[8][8];

  std::vector<CharacterOnBoard*> charactersOnBoard;
};

namespace errors
{
  struct field_not_found{};
}

#endif /* GAME_ENGINE_ACTIVEBOARD_H_ */
