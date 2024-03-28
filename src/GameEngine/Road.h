/**
 * Pola będące podglądem drogi jaką może przejść postać
*/

#ifndef GAME_ENGINE_ROAD_H_
#define GAME_ENGINE_ROAD_H_

#include <SFML/Graphics.hpp>

#include "FieldSet.h"

class Road : public FieldSet {
public:
    // konstuktor tworzący obiekt z pustą drogi
    Road();

    /**
     * \param _field zbiór pól z których składać się będzie droga
     * \param _start_field pierwsze pole drogi
    */
    Road(std::vector<Field*> _field, Field* _start_field);

    // zwraca odgległość w pikselach od poprzedniego pole do podanego
    sf::Vector2f getOffsetToThisField(Field* checkedField);

    // zwraca kierunek w jaki zwrócone jest poprzednie pole względem podanego
    Direction getDirectionToThisField(Field* checkedField) override;
private:
    // początkowe pole drogi
    Field* start_field;
};

#endif /* GAME_ENGINE_ROAD_H */