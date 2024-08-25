/**
 * Klasa bazowa zbiorów pól umożliwiających podświetlenie pól na planszy gry
 */

#ifndef GAME_ENGINE_FIELD_SET_
#define GAME_ENGINE_FIELD_SET_

#include <SFML/Graphics.hpp>

#include "Field.h"
#include "../Global.h"

class FieldSet : public sf::Drawable {
public:
  // zwraca wektor wksaźników do pól z których składa się na FieldSet
  std::vector<Field*> get();

  // zwraca ostatnio dodany element, czyli back() wektora
  Field* getLastElement();

  bool empty();
  void clear();

  // zwraca kierunek w jaki zwrócone jest poprzednie pole względem podanego
  virtual Direction getDirectionToThisField(Field* checkedField) = 0;

  void draw(sf::RenderTarget &target,
            sf::RenderStates states) const;
protected:
  // wskaźniki do pól z których składa się zbiór
  std::vector<Field*> field;

  // podświetlenia pól składających się na zbiór, graficzna reprezentacja przygotowana dla klas pochodnych FieldSet
  std::vector<sf::RectangleShape> hightlighOnFields;
};

// błędy jakie można napotkać
namespace errors
{
  struct field_not_found_in_set{};
}

#endif /* GAME_ENGINE_FIELD_SET_ */
