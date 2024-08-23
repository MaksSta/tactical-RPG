/**
 * Pola będące podglądaem zasięgu dla wywoływania umiejętności
 */

#ifndef GAME_ENGINE_RANGE_H_
#define GAME_ENGINE_RANGE_H_

#include <SFML/Graphics.hpp>

#include "FieldSet.h"

class Range : public FieldSet {
public:
  // konstuktor tworzący obiekt o pustym zasięgu pól
  Range();

  /**
   * @param _field zbiór pól z których składać się będzie zasięg
   * @param _cast_from_field pole z którego wywoływana jest umiejętność
   */
  Range(std::vector<Field*> _field,
        Field* _cast_from_field);

  // zwraca kierunek w jaki zwrócone jest poprzednie pole względem podanego
  Direction getDirectionToThisField(Field* checkedField);

  // zwraca pole z którego wywoływana jest umiejętność
  virtual Field* getCasterField();
private:
  Field* cast_from_field;
};

#endif /* GAME_ENGINE_RANGE_H_ */
