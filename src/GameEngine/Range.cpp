#include "Range.h"

Range::Range() 
:
    cast_from_field{nullptr}
{
}

Range::Range(std::vector<Field*> _field, Field* _cast_from_field)
:
    cast_from_field {_cast_from_field}
{
    field = _field;

    for( auto & f : field ) {
        sf::RectangleShape tmp_rect;
        tmp_rect.setSize(sf::Vector2f(tile_size, tile_size));
        tmp_rect.setFillColor(sf::Color{
            255, 0, 0, 24});
        tmp_rect.setPosition(f->getPosition());
        hightlighOnFields.push_back(tmp_rect);
    }
}

Direction Range::getDirectionToThisField(Field* checkedField)
{
    // uzyskanie wektora odległości od pola wywołania do sprawdzanego
    auto offset = checkedField->getPosition() - cast_from_field->getPosition();

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

Field* Range::getCasterField()
{
    return cast_from_field;
}