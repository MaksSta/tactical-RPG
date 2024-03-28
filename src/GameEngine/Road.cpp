#include "Road.h"

Road::Road()
:
    start_field {nullptr}
{
}

Road::Road(std::vector<Field*> _field, Field* _start_field)
:
    start_field {_start_field}
{
    field = _field;

    for( auto & f : field ) {
        sf::RectangleShape tmp_rect;
        tmp_rect.setSize(sf::Vector2f(tile_size, tile_size));
        tmp_rect.setFillColor(sf::Color{
            0, 255, 0, 24});
        tmp_rect.setPosition(f->getPosition());
        hightlighOnFields.push_back(tmp_rect);
    }
}

sf::Vector2f Road::getOffsetToThisField(Field* checkedField)
{
    // iterowanie po wszystkich polach począwszy od startowego, żeby znaleźć te które prowadzi na sprawdzane
    Field* previousField = start_field;
    for( auto & f : field)
    {
        if (f == checkedField)
            // zwrócenia przesunięcia od pole poprzedniego do sprawdzanego
            return checkedField->getPosition() - previousField->getPosition();

        previousField = f;
    }

    // szukane pole nie istnieje w zbiorze
    throw errors::field_not_found_in_set();
}

Direction Road::getDirectionToThisField(Field* checkedField)
{
    // uzyskanie wektora odległości sprawdzanego pola od poprzedniego pola drogi
    sf::Vector2f offset = getOffsetToThisField(checkedField);

    Direction direction;

    if (offset.x > 0) {
        direction = right;   
    }
    else if (offset.x < 0) {
        direction = left;
    }
    else if (offset.y > 0) {
        direction = down;
    }
    else if (offset.y < 0) {
        direction = up;
    }

    return direction;
}